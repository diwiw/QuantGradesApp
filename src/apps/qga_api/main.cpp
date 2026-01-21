#include <iostream>

#include "Version.hpp"
#include "api/ApiServer.hpp"
#include "core/Bootstrap.hpp"
#include "core/Config.hpp"
#include "utils/LoggerFactory.hpp"

int main()
{
    using namespace qga;

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

        // === Logger (async) ===
        auto logger = utils::LoggerFactory::createAsyncRotatingLogger(
            "api", (ctx.logDir / "qga_api.log").string(), ctx.cfg.logLevel(),
            ctx.cfg.logMaxSizeBytes(), ctx.cfg.logMaxFiles());

        ctx.cfg.setLogger(logger);
        logger->info("QuantGradesApp API starting... version={}", APP_VERSION);

        qga::api::ApiServer server(logger, ctx.cfg);
        server.start();
    }
    catch (const std::exception& e)
    {
        std::cerr << "[FATAL] API crashed: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
