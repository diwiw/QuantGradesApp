#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "utils/ILogger.hpp"

using namespace qga::utils;

// Minimal dummy implementation just to capture formatted strings
class DummyLogger : public ILogger
{
  public:
    std::string lastMessage;
    qga::LogLevel lastLevel;

    void log(qga::LogLevel level, const std::string& message) override
    {
        lastLevel = level;
        lastMessage = message;
    }
};

TEST(ILoggerTest, FormatsVariadicArgumentsCorrectly)
{
    DummyLogger logger;

    // Act
    logger.info("String: {}, Int: {}, Float: {:.2f}", "Hello", 42, 3.14159);

    // Assert
    EXPECT_EQ(logger.lastLevel, qga::LogLevel::Info);
    EXPECT_EQ(logger.lastMessage, "String: Hello, Int: 42, Float: 3.14");
}

TEST(ILoggerTest, ConvenienceMethodsRouteToCorrectLevel)
{
    DummyLogger logger;

    logger.trace("Trace msg");
    EXPECT_EQ(logger.lastLevel, qga::LogLevel::Trace);

    logger.debug("Debug msg");
    EXPECT_EQ(logger.lastLevel, qga::LogLevel::Debug);

    logger.error("Error msg");
    EXPECT_EQ(logger.lastLevel, qga::LogLevel::Err);

    logger.critical("Critical msg");
    EXPECT_EQ(logger.lastLevel, qga::LogLevel::Critical);
}
