// NOTE: This demo uses legacy-style runtime paths and logging.
//       It is not representative of production configuration.

#include <spdlog/async.h>

#include <iostream>
#include <memory>

#include "Version.hpp"
#include "core/Bootstrap.hpp"
#include "ingest/DataIngest.hpp"
#include "io/DataExporter.hpp"
#include "utils/SpdLogger.hpp"

using namespace qga;
using namespace utils;

int main()
{
    // === Header ===
    std::cout << "===================================\n";
    std::cout << " QuantGradesApp Logger Demo\n";
    std::cout << " Version: " << APP_VERSION << "\n";
    std::cout << " Build date: " << APP_BUILD_DATE << "\n";
    std::cout << "===================================\n\n";

    // === Bootstrap ===
    auto ctxOpt =
        core::bootstrapRuntime(std::filesystem::current_path(), core::AssetScope::LoggerDemo);

    if (!ctxOpt)
    {
        std::cerr << "[FATAL] Runtime bootstrap failed\n";
        return 1;
    }
    auto& ctx = *ctxOpt;
    auto& cfg = ctx.cfg;

    // ===== Logger Init =====
    if (!spdlog::thread_pool())
        spdlog::init_thread_pool(8192, 1); // start async thread pool

    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
        (ctx.logDir / cfg.logFile().filename()).string(), true);

    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto logger = std::make_shared<SpdLogger>(
        "LoggerDemo", std::vector<std::shared_ptr<spdlog::sinks::sink>>{console_sink, file_sink},
        true // async
    );

    logger->setLevel(cfg.logLevel());

    logger->info("[APP] Started (cfg={})", ctx.configPath.string());

    for (const auto& w : ctx.warnings)
        logger->warn("[Config] {}", w);

    logger->info("Logger initialized with level {}", toString(cfg.logLevel()));
    std::cout << "Config log level: " << toString(cfg.logLevel()) << "\n";
    // ===== 3. Ingest Data =====
    qga::ingest::DataIngest ingest(logger);

    const auto demo_csv = *ctx.assetsDir / "demo.csv";

    auto series = ingest.fromCsv(demo_csv.string());
    if (!series.has_value())
    {
        logger->error("Failed to ingest data from CSV");
        return 1;
    }
    else
    {
        logger->info("Ingested {} bars", series->size());
    }

    // ===== 4. Export Data =====

    try
    {
        namespace fs = std::filesystem;
        // fs::create_directories(cfg.dataDir()); // ensure output dir exists

        auto csv_path = ctx.dataDir / "demo_out.csv";
        auto json_path = ctx.dataDir / "demo_out.json";
        io::DataExporter exporter_csv(csv_path, logger, io::ExportFormat::CSV, false);
        exporter_csv.exportAll(*series);

        io::DataExporter exporter_json(json_path, logger, io::ExportFormat::JSON, false);
        exporter_json.exportAll(*series);

        logger->info("Exported series to CSV and JSON");
    }
    catch (const std::exception& ex)
    {
        logger->error("Exporter exception: {}", ex.what());
    }

    logger->info("APP] Logger demo finished successfully.");
    logger->flush();
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); //
    spdlog::shutdown();                                          // cleanup spdlog
    return 0;
}
