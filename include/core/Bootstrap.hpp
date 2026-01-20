/**
 * @file Bootstrap.hpp
 * @brief Shared runtime bootstrap for all QuantGradesApp executables.
 *
 * This module defines a unified bootstrap sequence used by all entrypoints
 * (API server, CLI tools, demos, future services).
 *
 * The bootstrap is responsible for:
 *  - locating the configuration file (ConfigLocator)
 *  - loading configuration in a deterministic order:
 *      defaults → config file → environment variables
 *  - optionally locating example or test assets (AssetsLocator)
 *  - preparing runtime directories (data, logs)
 *
 * The goal is to ensure that every executable starts with the same
 * well-defined runtime environment, independent of the current
 * working directory or repository layout.
 *
 * This bootstrap is considered stable for all 1.x releases.
 */

#pragma once
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include "core/AssetsLocator.hpp"
#include "core/Config.hpp"

namespace qga::core
{
    /**
     * @struct RuntimeContext
     * @brief Aggregated runtime state produced by the bootstrap process.
     *
     * RuntimeContext contains all information required by an executable
     * after bootstrap has completed:
     *  - the resolved configuration instance
     *  - collected configuration warnings
     *  - the resolved projectRoot
     *  - optional assets directory (for demos or tests)
     *  - the resolved configuration file path
     *
     * After bootstrap, executables should not perform any further
     * configuration discovery or filesystem probing.
     *
     * NOTE:
     * Logger is intentionally NOT initialized during bootstrap.
     * Logger initialization depends on resolved configuration
     * and must be performed after successful bootstrap.
     */
    struct RuntimeContext
    {
        core::Config& cfg;
        std::vector<std::string> warnings;
        std::filesystem::path projectRoot;
        std::optional<std::filesystem::path> assetsDir;
        std::filesystem::path configPath;
        std::filesystem::path dataDir;
        std::filesystem::path logDir;
    };

    /**
     * @brief Perform unified runtime bootstrap.
     *
     * This function executes the full runtime initialization sequence:
     *  - locate configuration file starting from the given directory
     *  - load configuration (defaults → file → environment)
     *  - optionally locate assets for a given asset scope
     *  - create required runtime directories (data, logs)
     *
     * On failure, the function reports a fatal error to stderr and
     * returns std::nullopt.
     *
     * @param startDir Directory from which configuration and assets discovery begins.
     * @param assetsScope Optional asset scope (used by demos or tests).
     * @param createRuntimeDirs Whether runtime directories should be created automatically.
     *
     * @return RuntimeContext on success, std::nullopt on fatal bootstrap failure.
     */
    std::optional<RuntimeContext>
    bootstrapRuntime(const std::filesystem::path& startDir,
                     std::optional<core::AssetScope> assetsScope = std::nullopt,
                     bool createRuntimeDirs = true);
} // namespace qga::core
