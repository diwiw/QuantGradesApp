#include "AppCLI.hpp"

#ifndef USE_BOOST
#include <CLI/CLI.hpp>
#else
#include <boost/program_options.hpp>
namespace po = boost::program_options;
#endif

#include <fmt/core.h>

#include <filesystem>
#include <iostream>

#include "Version.hpp"
#include "common/LogLevel.hpp"
#include "core/Config.hpp"
#include "domain/backtest/Engine.hpp"
#include "ingest/DataIngest.hpp"
#include "io/DataExporter.hpp"
#include "strategy/BuyHold.hpp"
#include "strategy/MACrossover.hpp"
#include "utils/ILogger.hpp"
#include "utils/LoggerFactory.hpp"

namespace qga::cli
{

    AppCLI::AppCLI() = default;
    AppCLI::~AppCLI() = default;

    int AppCLI::run(int argc, char** argv)
    {
#ifndef USE_BOOST
        CLI::App app{"QuantGradesApp CLI - run backtests and simulations"};

        std::string config_path;
        std::string cli_input;
        std::string cli_output;
        bool show_version = false;

        app.add_flag("--version", show_version, "Show version information");
        app.add_option("--config", config_path, "Path to configuration file");

        app.add_option("--input", cli_input, "Override input data file (CSV)");
        app.add_option("--output", cli_output, "Override output results file (CSV)");

        CLI11_PARSE(app, argc, argv);

        if (show_version)
        {
            std::cout << "QuantGradesApp " << APP_VERSION << " (" << APP_BUILD_DATE << ")\n";
            return 0;
        }

        if (config_path.empty())
        {
            std::cerr << "ERROR: --config is required\n";
            return 1;
        }
#else
        po::options_description desc("QuantGradesApp CLI options");
        desc.add_options()("help,h", "Show help")("version,v", "Version")(
            "config,c", po::value<std::string>(),
            "Config file")("input,i", po::value<std::string>(),
                           "Input CSV")("output,o", po::value<std::string>(), "Output CSV");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help"))
        {
            std::cout << desc << "\n";
            return 0;
        }

        if (vm.count("version"))
        {
            std::cout << "QuantGradesApp " << APP_VERSION << "\n";
            return 0;
        }

        if (!vm.count("config"))
        {
            std::cerr << "ERROR: --config is required\n";
            return 1;
        }

        std::string config_path = vm["config"].as<std::string>();
        std::string cli_input = vm.count("input") ? vm["input"].as<std::string>() : "";
        std::string cli_output = vm.count("output") ? vm["output"].as<std::string>() : "";
#endif

        // -----------------------------------------------------
        // Load config
        // -----------------------------------------------------
        auto& config = qga::core::Config::getInstance();

        try
        {
            config.loadFromFile(config_path);
            std::cout << "Loaded configuration: " << config_path << "\n";
        }
        catch (const std::exception& ex)
        {
            std::cerr << "ERROR: Failed to load configuration: " << ex.what() << "\n";
            return 1;
        }

        // -----------------------------------------------------
        // CLI overrides
        // -----------------------------------------------------
        if (!cli_input.empty())
            config.setInputPath(cli_input);

        if (!cli_output.empty())
            config.setOutputPath(cli_output);

        // -----------------------------------------------------
        // Validate input path
        // -----------------------------------------------------
        if (config.inputPath().empty())
        {
            std::cerr << "ERROR: missing input.path\n";
            return 1;
        }

        if (!std::filesystem::exists(config.inputPath()))
        {
            std::cerr << "ERROR: Input file does not exist: " << config.inputPath() << "\n";
            return 1;
        }

        // -----------------------------------------------------
        // CLI logger (SYNC)
        // -----------------------------------------------------
        auto logger = qga::utils::LoggerFactory::createConsoleLogger("CLI", qga::LogLevel::Info);
        logger->info(fmt::format("CLI started with input={}", config.inputPath().string()));

        // -----------------------------------------------------
        // Load CSV
        // -----------------------------------------------------
        qga::ingest::DataIngest ingest(logger);
        auto series_opt = ingest.fromCsv(config.inputPath().string());

        if (!series_opt.has_value())
        {
            std::cerr << "ERROR: Failed to load input CSV: " << config.inputPath().string() << "\n";
            return 1;
        }

        auto series = *series_opt;

        // -----------------------------------------------------
        // Run strategy + engine
        // -----------------------------------------------------
        qga::strategy::BuyHold strat{};
        qga::domain::backtest::Engine engine(10000.0);

        auto result = engine.run(series, strat);
        logger->info(fmt::format("Backtest finished. Final equity={}", result.final_equity_));

        // -----------------------------------------------------
        // Export results
        // -----------------------------------------------------
        if (config.outputPath().empty())
        {
            std::cerr << "ERROR: missing output.path\n";
            return 1;
        }

        qga::io::DataExporter exporter(config.outputPath().string(), logger,
                                       qga::io::ExportFormat::CSV, false);

        try
        {
            exporter.exportAll(series);
        }
        catch (const std::exception& ex)
        {
            std::cerr << "ERROR: Export failed: " << ex.what() << "\n";
            return 1;
        }

        std::cout << "Results exported to: " << config.outputPath().string() << "\n";

        return 0;
    }

} // namespace qga::cli
