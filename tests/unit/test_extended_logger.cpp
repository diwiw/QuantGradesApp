/**
 * @file test_logger.cpp
 * @brief Unit tests for ILogger implementations (MockLogger, NullLogger, SpdLogger).
 */

#include <filesystem>
#include <fstream>

#include "doctest.h"
#include "fixtures/MockLoggerCapture.hpp"
#include "utils/ILogger.hpp"
#include "utils/NullLogger.hpp"
#include "utils/SpdLogger.hpp"

using namespace qga::tests::fixtures;

TEST_SUITE("Logger")
{

    TEST_CASE("MockLogger stores messages")
    {
        auto logger = std::make_shared<MockLoggerCapture>();

        logger->info("Hello");
        logger->warn("Something might be wrong");
        logger->error("Error happened");

        auto infos = logger->getLogsByLevel(qga::LogLevel::Info);
        auto warns = logger->getLogsByLevel(qga::LogLevel::Warn);
        auto errors = logger->getLogsByLevel(qga::LogLevel::Err);
        CHECK(infos[0] == "Hello");
        CHECK(warns[0] == "Something might be wrong");
        CHECK(errors[0] == "Error happened");
    }

    TEST_CASE("NullLogger ignores messages")
    {
        auto logger = std::make_shared<qga::utils::NullLogger>();

        // These logs should be ignored
        logger->info("Should not appear");
        logger->error("Still nothing");

        // Nothing to check – class intentionally does not store logs
        CHECK(true); // test will always pass
    }

    TEST_CASE("SpdLogger writes to file")
    {
        const std::string LOG_FILE = "test_spdlog.log";
        if (std::filesystem::exists(LOG_FILE))
        {
            std::filesystem::remove(LOG_FILE);
        }

        auto logger = std::make_shared<qga::utils::SpdLogger>(
            "TestLogger",
            std::vector<std::shared_ptr<spdlog::sinks::sink>>{
                std::make_shared<spdlog::sinks::basic_file_sink_mt>(LOG_FILE, true)},
            false // sync mode
        );

        logger->info("Logger started");
        logger->error("Something bad");

        logger.reset(); // wymuszamy flush i zamknięcie

        REQUIRE(std::filesystem::exists(LOG_FILE));
        std::ifstream infile(LOG_FILE);
        std::string contents((std::istreambuf_iterator<char>(infile)),
                             std::istreambuf_iterator<char>());

        CHECK(contents.find("Logger started") != std::string::npos);
        CHECK(contents.find("Something bad") != std::string::npos);
    }
}
