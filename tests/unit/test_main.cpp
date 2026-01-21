#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <filesystem>
#include <iostream>
#include <memory>

#include "Version.hpp"
#include "doctest.h"
#include "utils/LoggerFactory.hpp"
#include "utils/SpdLogger.hpp"

using namespace qga;
using namespace utils;

/**
 * @brief Global fixture for initializing the logger before all tests.
 */
struct LogInit
{
    std::shared_ptr<ILogger> logger_;

    LogInit()
    {
        // Ensure logs directory exists
        if (!std::filesystem::exists("logs"))
        {
            std::filesystem::create_directory("logs");
        }

        // Create async logger via LoggerFactory
        logger_ =
            LoggerFactory::createLogger("tests_logger", "logs/test_log.txt", qga::LogLevel::Debug);
        logger_->info("[TEST] Logger initialized successfully.");
    }
};

// Global instance, initialized before running any test
static LogInit log_init;

/**
 * @brief Displays app version info at test startup.
 */
struct VersionPrinter
{
    VersionPrinter()
    {
        std::cout << "===================================\n";
        std::cout << " QuantGradesApp Tests\n";
        std::cout << " Version: " << APP_VERSION << "\n";
        std::cout << " Build date: " << APP_BUILD_DATE << "\n";
        std::cout << "===================================\n\n";
    }
};

// Global static instance to print version info before tests
static VersionPrinter version_printer;
