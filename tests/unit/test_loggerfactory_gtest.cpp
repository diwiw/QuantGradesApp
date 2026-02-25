#include <gtest/gtest.h>

#include <filesystem>
#include <memory>
#include <string>

#include "fixtures/BaseTestFixture.hpp"
#include "utils/LoggerFactory.hpp"
#include "utils/SpdLogger.hpp"

using namespace qga::utils;
using namespace qga::tests::fixtures;

class LoggerFactoryTest : public BaseTestFixture
{
};

TEST_F(LoggerFactoryTest, CreatesNullLogger)
{
    auto logger = LoggerFactory::createNullLogger();

    ASSERT_NE(logger, nullptr);

    auto spdLogger = std::dynamic_pointer_cast<SpdLogger>(logger);
    EXPECT_NE(spdLogger, nullptr);

    EXPECT_NO_FATAL_FAILURE({
        logger->info("This goes to the void");
        logger->flush();
    });
}

TEST_F(LoggerFactoryTest, CreatesConsoleLogger)
{
    auto logger = LoggerFactory::createConsoleLogger("ConsoleLoggerTest", qga::LogLevel::Debug);

    ASSERT_NE(logger, nullptr);

    auto spdLogger = std::dynamic_pointer_cast<SpdLogger>(logger);
    EXPECT_NE(spdLogger, nullptr);

    EXPECT_NO_FATAL_FAILURE({ logger->debug("Console debug test"); });
}

TEST_F(LoggerFactoryTest, CreatesStandardFileLogger)
{
    // Hardkodowana nazwa tylko na potrzeby TEGO konkretnego testu
    const std::string dummyLogFile = "dummy_standard_factory.log";
    trackFile(dummyLogFile); // Zlecenie sprzątania po teście!

    {
        auto logger =
            LoggerFactory::createLogger("FileLoggerTest", dummyLogFile, qga::LogLevel::Info);

        ASSERT_NE(logger, nullptr);
        auto spdLogger = std::dynamic_pointer_cast<SpdLogger>(logger);
        EXPECT_NE(spdLogger, nullptr);

        logger->info("Test standard file logger");
        logger->flush();
    } // Wyjście z klamry zamyka loggera i zwalnia uchwyt do pliku

    // Weryfikacja
    EXPECT_TRUE(std::filesystem::exists(dummyLogFile));
}

TEST_F(LoggerFactoryTest, CreatesAsyncRotatingLogger)
{
    const std::string dummyRotatingFile = "dummy_rotating_factory.log";
    trackFile(dummyRotatingFile); // Zlecenie sprzątania po teście!

    {
        auto logger = LoggerFactory::createAsyncRotatingLogger("AsyncLoggerTest", dummyRotatingFile,
                                                               qga::LogLevel::Warn,
                                                               1024 * 1024, // 1MB
                                                               2            // 2 pliki rotacyjne
        );

        ASSERT_NE(logger, nullptr);

        logger->warn("Test async rotating logger");
        logger->flush();
    }

    // Weryfikacja
    EXPECT_TRUE(std::filesystem::exists(dummyRotatingFile));
}
