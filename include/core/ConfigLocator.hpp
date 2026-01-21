/**
 * @file ConfigLocator.hpp
 * @brief Filesystem-based configuration discovery.
 *
 * ConfigLocator provides a deterministic mechanism for locating
 * the main configuration file (config.json) by traversing parent
 * directories starting from a given path.
 *
 * This allows executables to be launched from arbitrary locations
 * (e.g. build directories, installed binaries, CI environments)
 * without relying on the current working directory or hardcoded paths.
 *
 * Configuration discovery is strictly a bootstrap concern and must
 * not be used during normal runtime logic.
 */

#pragma once
#include <filesystem>
#include <optional>

namespace qga::core
{
    /**
     * @brief Locate the configuration file by traversing parent directories.
     *
     * Starting from the given directory, this function searches upwards
     * for a valid QuantGradesApp configuration file (e.g. config/config.json).
     *
     * The search stops at the filesystem root.
     *
     * @param startDir Directory from which the search begins.
     * @return Path to the configuration file if found, std::nullopt otherwise.
     */
    std::optional<std::filesystem::path> findConfigFile(const std::filesystem::path& startDir);

}