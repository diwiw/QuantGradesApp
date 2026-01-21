/**
 * @file Config.hpp
 * @brief Global singleton configuration for QuantGradesApp.
 *
 * Supports:
 * - JSON-based config loading: logging, paths, engine, api
 * - Profile-based config (config.dev.json, config.prod.json, ...)
 * - Environment variable overrides (QGA_*)
 * - Integrated rotating logger
 */

#pragma once
#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "common/LogLevel.hpp"
#include "utils/ILogger.hpp"

namespace qga::core
{

    class Config
    {
      public:
        // ============================================================
        // Singleton access
        // ============================================================
        static Config& getInstance() noexcept;

        // ============================================================
        // Loading
        // ============================================================
        void loadFromFile(const std::filesystem::path& path,
                          std::vector<std::string>* warnings = nullptr);

        void loadFromEnv(std::vector<std::string>* warnings = nullptr);

        void loadDefaults();
        void validate(std::vector<std::string>* warnings = nullptr);

        // ============================================================
        // API — read-only getters
        // ============================================================
        const std::string& profile() const noexcept { return profile_; }

        // --- API server ---
        int apiPort() const noexcept { return api_port_; }

        // --- Engine settings ---
        int threads() const noexcept { return threads_; }

        // --- Paths ---
        const std::filesystem::path& dataDir() const noexcept { return data_dir_; }

        // --- Logging ---
        LogLevel logLevel() const noexcept { return log_level_; }
        const std::filesystem::path& logFile() const noexcept { return log_file_; }
        size_t logMaxSizeBytes() const noexcept { return log_max_size_mb_ * 1024 * 1024; }
        size_t logMaxFiles() const noexcept { return log_max_files_; }

        // --- Version ---
        const std::string& version() const noexcept { return version_; }

        // --- CLI / ingest ---
        const std::filesystem::path& inputPath() const noexcept { return input_path_; }
        const std::filesystem::path& outputPath() const noexcept { return output_path_; }

        void setInputPath(const std::filesystem::path& p) { input_path_ = p; }
        void setOutputPath(const std::filesystem::path& p) { output_path_ = p; }

        // ============================================================
        // No copying
        // ============================================================
        ~Config() = default;
        Config(const Config&) = delete;
        Config(Config&&) = delete;
        Config& operator=(const Config&) = delete;
        Config& operator=(Config&&) = delete;

        // ============================================================
        // Logger access
        // ============================================================
        void setLogger(std::shared_ptr<utils::ILogger> logger) { logger_ = std::move(logger); }

      private:
        Config() = default;

        // Helpers
        static std::string toLower(std::string s) noexcept;
        static void addWarn(std::vector<std::string>* w, std::string msg);

      private:
        // ============================================================
        // Stored fields
        // ============================================================

        std::string profile_;

        // API server
        int api_port_ = 8080;

        // Engine
        int threads_ = 4;

        // Paths
        std::filesystem::path data_dir_ = "data";

        // Logging
        LogLevel log_level_ = LogLevel::Info;
        std::filesystem::path log_file_ = "logs/qga.log";
        size_t log_max_size_mb_ = 10;
        size_t log_max_files_ = 3;

        // Version (loaded from Version.hpp)
        std::string version_ = "0.0.0";

        // CLI / ingest
        std::filesystem::path input_path_ = "";
        std::filesystem::path output_path_ = "";

        // Logger
        std::shared_ptr<utils::ILogger> logger_;
    };

} // namespace qga::core
