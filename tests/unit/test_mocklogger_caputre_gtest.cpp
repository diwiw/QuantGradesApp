#include <gtest/gtest.h>

#include "fixtures/MockLoggerCapture.hpp"

using namespace qga::tests::fixtures;

// ==============================================================================
// 1. Basic Logging and Filtering
// ==============================================================================

TEST(MockLoggerCaptureTest, StoresMessagesCorrectly)
{
    MockLoggerCapture logger;
    logger.setLevel(qga::LogLevel::Trace); // Allow all levels to pass through

    logger.info("Hello");
    logger.warn("Something might be wrong");
    logger.error("Error happened");

    auto infos = logger.getLogsByLevel(qga::LogLevel::Info);
    auto warns = logger.getLogsByLevel(qga::LogLevel::Warn);
    auto errors = logger.getLogsByLevel(qga::LogLevel::Err);

    ASSERT_EQ(infos.size(), 1);
    EXPECT_EQ(infos[0], "Hello");

    ASSERT_EQ(warns.size(), 1);
    EXPECT_EQ(warns[0], "Something might be wrong");

    ASSERT_EQ(errors.size(), 1);
    EXPECT_EQ(errors[0], "Error happened");
}

TEST(MockLoggerCaptureTest, FiltersMessagesByLevelBoundaries)
{
    MockLoggerCapture logger;

    // Edge case: Block everything below ERROR
    logger.setLevel(qga::LogLevel::Err);

    logger.debug("Ignored debug");
    logger.info("Ignored info");
    logger.error("Captured error");

    EXPECT_EQ(logger.entries().size(), 1);
    EXPECT_TRUE(logger.contains(qga::LogLevel::Err, "Captured error"));
    EXPECT_FALSE(logger.contains(qga::LogLevel::Debug, "Ignored debug"));
}

// ==============================================================================
// 2. Additional Methods (for high test coverage)
// ==============================================================================

TEST(MockLoggerCaptureTest, ClearRemovesAllEntries)
{
    MockLoggerCapture logger;
    logger.setLevel(qga::LogLevel::Trace);

    // Arrange
    logger.info("Message 1");
    logger.error("Message 2");
    EXPECT_EQ(logger.entries().size(), 2);

    // Act
    logger.clear();

    // Assert
    EXPECT_TRUE(logger.entries().empty());
    EXPECT_EQ(logger.count(qga::LogLevel::Info), 0);
}

TEST(MockLoggerCaptureTest, CountReturnsCorrectNumber)
{
    MockLoggerCapture logger;
    logger.setLevel(qga::LogLevel::Trace);

    logger.info("Info 1");
    logger.info("Info 2");
    logger.error("Error 1");

    EXPECT_EQ(logger.count(qga::LogLevel::Info), 2);
    EXPECT_EQ(logger.count(qga::LogLevel::Err), 1);
    EXPECT_EQ(logger.count(qga::LogLevel::Warn), 0); // Level without any logs
}

TEST(MockLoggerCaptureTest, AllLogsReturnsPreservedOrderStrings)
{
    MockLoggerCapture logger;
    logger.setLevel(qga::LogLevel::Trace);

    logger.debug("First");
    logger.info("Second");
    logger.error("Third");

    auto logs = logger.allLogs();

    ASSERT_EQ(logs.size(), 3);
    EXPECT_EQ(logs[0], "First");
    EXPECT_EQ(logs[1], "Second");
    EXPECT_EQ(logs[2], "Third");
}

TEST(MockLoggerCaptureTest, LastReturnsCorrectEntry)
{
    MockLoggerCapture logger;
    logger.setLevel(qga::LogLevel::Trace);

    // 1. Verify behavior on an empty buffer (should return nullopt)
    EXPECT_FALSE(logger.last().has_value());

    // 2. Add some logs
    logger.info("Old message");
    logger.error("Newest message");

    // 3. Verify it retrieves the last added log
    auto lastEntry = logger.last();
    ASSERT_TRUE(lastEntry.has_value());
    EXPECT_EQ(lastEntry->level, qga::LogLevel::Err);
    EXPECT_EQ(lastEntry->msg, "Newest message");
}
