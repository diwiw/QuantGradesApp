#include <gtest/gtest.h>

#include "fixtures/MockLoggerCapture.hpp"

TEST(MockLoggerCapture, CapturesAndFiltersLogMessages)
{
    qga::tests::fixtures::MockLoggerCapture logger;

    // Simulate logging at different levels
    logger.log(qga::LogLevel::Debug, "Debug message");
    logger.log(qga::LogLevel::Info, "Info message");
    logger.log(qga::LogLevel::Warn, "Warning message");
    logger.log(qga::LogLevel::Err, "Error message");

    // Set minimum log level to WARNING
    logger.setLevel(qga::LogLevel::Warn);

    // Check that only WARNING and ERROR messages are captured
    EXPECT_TRUE(logger.contains(qga::LogLevel::Warn, "Warning message"));
    EXPECT_TRUE(logger.contains(qga::LogLevel::Err, "Error message"));
    EXPECT_FALSE(logger.contains(qga::LogLevel::Debug, "Debug message"));
    EXPECT_FALSE(logger.contains(qga::LogLevel::Info, "Info message"));
}
