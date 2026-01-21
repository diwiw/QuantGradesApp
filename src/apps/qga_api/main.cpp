#include <spdlog/spdlog.h>

#include <csignal>
#include <iostream>

#include "Version.hpp"
#include "api/ApiServer.hpp"
#include "core/Bootstrap.hpp"
#include "core/Config.hpp"
#include "utils/LoggerFactory.hpp"

int main()
{
    using namespace qga;

    // === Header ===
    std::cout << "===================================\n";
    std::cout << " QuantGradesApp API\n";
    std::cout << " Version: " << APP_VERSION << "\n";
    std::cout << " Build date: " << APP_BUILD_DATE << "\n";
    std::cout << "===================================\n\n";

    try
    {
        // === Bootstrap runtime ===
        auto ctxOpt = core::bootstrapRuntime(std::filesystem::current_path());
        if (!ctxOpt)
        {
            std::cerr << "[FATAL] API bootstrap failed\n";
            return 1;
        }

        auto& ctx = *ctxOpt;
        auto& cfg = ctx.cfg;

        // === Logger (async) ===
        auto logger = utils::LoggerFactory::createAsyncRotatingLogger(
            "qga_api", ctx.logDir / cfg.logFile().filename(), cfg.logLevel(), cfg.logMaxSizeBytes(),
            cfg.logMaxFiles());

        ctx.cfg.setLogger(logger);
        logger->info("QuantGradesApp API starting... version={}", APP_VERSION);

        qga::api::ApiServer server(logger, cfg);

        // ---- graceful shutdown ----
        std::signal(SIGINT,
                    [](int)
                    {
                        spdlog::shutdown();
                        std::exit(0);
                    });

        std::signal(SIGTERM,
                    [](int)
                    {
                        spdlog::shutdown();
                        std::exit(0);
                    });
        server.start();
    }
    catch (const std::exception& e)
    {
        std::cerr << "[FATAL] API crashed: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
