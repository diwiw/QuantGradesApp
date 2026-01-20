#include "core/Bootstrap.hpp"

#include <iostream>

#include "core/ConfigLocator.hpp"

namespace fs = std::filesystem;

namespace qga::core
{
    std::optional<RuntimeContext> bootstrapRuntime(const fs::path& startDir,
                                                   std::optional<core::AssetScope> assetsScope,
                                                   bool createRuntimeDirs)
    {
        RuntimeContext ctx{.cfg = core::Config::getInstance(),
                           .warnings = {},
                           .projectRoot = {},
                           .assetsDir = std::nullopt,
                           .configPath = {},
                           .dataDir = {},
                           .logDir = {}};

        // 1) Find config
        auto cfgPath = core::findConfigFile(startDir);
        if (!cfgPath)
        {
            std::cerr << "[FATAL] config.json not found (startDir=" << fs::absolute(startDir)
                      << ")\n";
            return std::nullopt;
        }
        ctx.configPath = *cfgPath;
        ctx.projectRoot = ctx.configPath.parent_path().parent_path();

        // 2) Load config: defaults -> file -> env
        ctx.cfg.loadDefaults();
        ctx.cfg.loadFromFile(ctx.configPath, &ctx.warnings);
        ctx.cfg.loadFromEnv(&ctx.warnings);

        // 3) Find assets (optional)
        if (assetsScope)
        {
            ctx.assetsDir = core::findAssetsDir(*assetsScope, startDir);
            if (!ctx.assetsDir)
            {
                std::cerr << "[FATAL] assets not found for requested scope\n";
                return std::nullopt;
            }
        }

        // 4) Prepare runtime dirs (optional)
        auto dataCfg = ctx.cfg.dataDir();
        ctx.dataDir = dataCfg.is_relative() ? ctx.projectRoot / dataCfg : dataCfg;

        auto logFile = ctx.cfg.logFile();
        ctx.logDir =
            logFile.is_relative() ? ctx.projectRoot / logFile.parent_path() : logFile.parent_path();

        if (createRuntimeDirs)
        {
            try
            {
                fs::create_directories(ctx.dataDir);
                fs::create_directories(ctx.logDir);
            }
            catch (const std::exception& e)
            {
                std::cerr << "[FATAL] Cannot create runtime directories: " << e.what() << "\n";
                return std::nullopt;
            }
        }

        return ctx;
    }
} // namespace qga::core
