#include <gtest/gtest.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <filesystem>
#include <fstream>
#include <memory>
#include <string>

#include "fixtures/BaseTestFixture.hpp"
#include "utils/SpdLogger.hpp"

using namespace qga::utils;
using namespace qga::tests::fixtures;

class SpdLoggerTest : public BaseTestFixture
{
};

TEST_F(SpdLoggerTest, WritesMessagesToUnderlyingSink)
{
    const std::string testFile = "dummy_spdlogger_test.log";
    trackFile(testFile);

    // Arrange: Create a SpdLogger directly with a file sink
    auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(testFile, true);
    auto logger = std::make_shared<SpdLogger>("TestLogger", std::vector<spdlog::sink_ptr>{sink});

    // Act
    logger->info("This is an info message");
    logger->error("This is an error message");
    logger->flush();

    // Assert: Check if file exists and contains the messages
    ASSERT_TRUE(std::filesystem::exists(testFile));

    std::ifstream infile(testFile);
    std::string contents((std::istreambuf_iterator<char>(infile)),
                         std::istreambuf_iterator<char>());

    EXPECT_NE(contents.find("This is an info message"), std::string::npos);
    EXPECT_NE(contents.find("This is an error message"), std::string::npos);
}

TEST_F(SpdLoggerTest, RespectsLogLevel)
{
    const std::string testFile = "dummy_spdlogger_level_test.log";
    trackFile(testFile);

    auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(testFile, true);
    auto logger = std::make_shared<SpdLogger>("TestLogger", std::vector<spdlog::sink_ptr>{sink});

    // Set level to WARNING, so INFO should be ignored
    logger->setLevel(qga::LogLevel::Warn);

    logger->info("Hidden info message");
    logger->warn("Visible warning message");
    logger->flush();

    ASSERT_TRUE(std::filesystem::exists(testFile));
    std::ifstream infile(testFile);
    std::string contents((std::istreambuf_iterator<char>(infile)),
                         std::istreambuf_iterator<char>());

    // Verify INFO was blocked and WARN passed
    EXPECT_EQ(contents.find("Hidden info message"), std::string::npos);
    EXPECT_NE(contents.find("Visible warning message"), std::string::npos);
}
