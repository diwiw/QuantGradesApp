/**
 * @file AssetsLocator.hpp
 * @brief Discovery of read-only assets for demos and tests.
 *
 * AssetsLocator is responsible for locating example or test assets
 * (input data, reference files) associated with a specific asset scope.
 *
 * Assets are treated as read-only resources and are strictly separated
 * from runtime output directories (data, logs).
 *
 * This mechanism allows demos and tests to remain deterministic and
 * independent of the current working directory or installation layout.
 */

#pragma once
#include <filesystem>
#include <optional>

namespace qga::core
{
    /**
     * @enum AssetScope
     * @brief Logical scope of assets used for discovery.
     *
     * AssetScope defines the semantic owner of a given assets directory
     * (e.g. a specific demo or test suite).
     *
     * Each scope corresponds to a well-defined subdirectory under
     * the repository's examples or tests hierarchy.
     */
    enum class AssetScope
    {
        GradesDemo,
        LoggerDemo,
        BacktestDemo
    };

    /**
     * @brief Locate the assets directory for a given asset scope.
     *
     * Starting from the provided directory, this function searches
     * upwards for a matching assets directory corresponding to the
     * specified asset scope.
     *
     * The search strategy is intentionally filesystem-based to avoid
     * hardcoded paths and to support execution from arbitrary locations.
     *
     * @param scope Asset scope identifying which assets to locate.
     * @param startDir Directory from which the search begins.
     * @return Path to the assets directory if found, std::nullopt otherwise.
     */
    std::optional<std::filesystem::path> findAssetsDir(AssetScope scope,
                                                       const std::filesystem::path& startDir);
} // namespace qga::core
