/**
 * @file LoggerFactory.hpp
 * @brief Factory class for creating loggers.
 *
 * This class provides methods to create different types of loggers
 * (e.g., console, file, null) with specific configurations.
 *
 * Designed to be injected into classes expecting an ILogger.
 * Uses shared sinks for file and console output.
 *
 * @author
 * @date 2025
 */

#pragma once

#include <spdlog/async.h>
#include <spdlog/async_logger.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/null_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <memory>
#include <string>

#include "common/LogLevel.hpp"
#include "utils/ILogger.hpp"

namespace qga::utils
{

    /**
     * @class LoggerFactory
     * @brief Factory for creating spdlog-based loggers.
     *
     * Handles creation of async/file/stdout/null loggers with custom settings.
     * Allows easy integration with Config system and future extension.
     */
    class LoggerFactory
    {
      public:
        /**
         * @brief Creates a standard asynchronous file logger.
         * @param name Logger name (e.g. "core", "engine").
         * @param filename Output file path.
         * @param level LogLevel threshold.
         * @return Shared pointer to ILogger.
         */
        static std::shared_ptr<ILogger>
        createLogger(const std::string& name, const std::string& filename, qga::LogLevel level);

        /**
         * @brief Creates an asynchronous rotating logger.
         *
         * @param name Logger name (e.g. "core", "engine").
         * @param filename Output file path.
         * @param level LogLevel threshold.
         * @param max_size Max size in bytes before rotation.
         * @param max_files Number of rotated files to keep.
         * @return Shared pointer to ILogger.
         */
        static std::shared_ptr<ILogger>
        createAsyncRotatingLogger(const std::string& name, const std::string& filename,
                                  qga::LogLevel level = qga::LogLevel::Info,
                                  size_t max_size = 1048576 * 5, // 5MB
                                  size_t max_files = 3);

        /**
         * @brief Creates a stdout-only logger for CLI/debugging.
         * @param name Logger name.
         * @param level Log level threshold.
         * @return Shared pointer to ILogger.
         */
        static std::shared_ptr<ILogger>
        createConsoleLogger(const std::string& name, qga::LogLevel level = qga::LogLevel::Info);

        /**
         * @brief Creates a disabled/null logger (e.g. for testing).
         * @return Shared pointer to ILogger that discards messages.
         */
        static std::shared_ptr<ILogger> createNullLogger();
    };

} // namespace qga::utils
