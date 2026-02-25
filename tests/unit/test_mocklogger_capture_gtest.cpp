#include <gtest/gtest.h>

#include "fixtures/MockLoggerCapture.hpp"
// Zakładam, że ILogger.hpp lub MockLoggerCapture.hpp odpowiednio includuje LogLevel.hpp

namespace qga::tests::fixtures::test
{

    class MockLoggerCaptureTest : public ::testing::Test
    {
      protected:
        MockLoggerCapture logger;

        void SetUp() override
        {
            // Na starcie ustawiamy poziom na najniższy (Trace/Debug), żeby łapać wszystko
            logger.setLevel(qga::LogLevel::Debug);
        }
    };

    TEST_F(MockLoggerCaptureTest, IsInitiallyEmpty)
    {
        EXPECT_TRUE(logger.entries().empty());
        EXPECT_TRUE(logger.allLogs().empty());
        EXPECT_EQ(logger.count(qga::LogLevel::Info), 0);
        EXPECT_FALSE(logger.last().has_value());
    }

    TEST_F(MockLoggerCaptureTest, CapturesAndStoresLogs)
    {
        logger.log(qga::LogLevel::Info, "Message 1");
        logger.log(qga::LogLevel::Err, "Message 2"); // Zauważ użycie ::Err

        auto entries = logger.entries();
        ASSERT_EQ(entries.size(), 2);

        EXPECT_EQ(entries[0].level, qga::LogLevel::Info);
        EXPECT_EQ(entries[0].msg, "Message 1");

        EXPECT_EQ(entries[1].level, qga::LogLevel::Err);
        EXPECT_EQ(entries[1].msg, "Message 2");
    }

    TEST_F(MockLoggerCaptureTest, ContainsFindsSubstringCorrectly)
    {
        logger.log(qga::LogLevel::Info, "User logged in successfully");
        logger.log(qga::LogLevel::Err, "Connection timeout occurred");

        // Pełne trafienia i częściowe (substring)
        EXPECT_TRUE(logger.contains(qga::LogLevel::Info, "logged in"));
        EXPECT_TRUE(logger.contains(qga::LogLevel::Err, "timeout"));

        // Prawidłowa wiadomość, ale zły poziom logowania
        EXPECT_FALSE(logger.contains(qga::LogLevel::Debug, "logged in"));

        // Prawidłowy poziom, ale brakujące słowo
        EXPECT_FALSE(logger.contains(qga::LogLevel::Info, "failed"));
    }

    TEST_F(MockLoggerCaptureTest, CountReturnsCorrectNumberByLevel)
    {
        logger.log(qga::LogLevel::Info, "Info 1");
        logger.log(qga::LogLevel::Info, "Info 2");
        logger.log(qga::LogLevel::Err, "Error 1");

        EXPECT_EQ(logger.count(qga::LogLevel::Info), 2);
        EXPECT_EQ(logger.count(qga::LogLevel::Err), 1);
        EXPECT_EQ(logger.count(qga::LogLevel::Debug), 0);
    }

    TEST_F(MockLoggerCaptureTest, LastReturnsLatestEntry)
    {
        logger.log(qga::LogLevel::Info, "First log");
        logger.log(qga::LogLevel::Err, "Second log");

        auto lastEntry = logger.last();
        ASSERT_TRUE(lastEntry.has_value());
        EXPECT_EQ(lastEntry->level, qga::LogLevel::Err);
        EXPECT_EQ(lastEntry->msg, "Second log");
    }

    TEST_F(MockLoggerCaptureTest, ClearRemovesAllData)
    {
        logger.log(qga::LogLevel::Info, "Some log");
        EXPECT_EQ(logger.entries().size(), 1);

        logger.clear();

        EXPECT_TRUE(logger.entries().empty());
        EXPECT_FALSE(logger.last().has_value());
    }

    TEST_F(MockLoggerCaptureTest, SetLevelFiltersOutLowerLevelLogs)
    {
        // Ustawiamy limit na Info (odrzucamy Trace i Debug)
        logger.setLevel(qga::LogLevel::Info);

        logger.log(qga::LogLevel::Debug, "This should be ignored");
        logger.log(qga::LogLevel::Info, "This should be captured");
        logger.log(qga::LogLevel::Err, "This should also be captured");

        auto entries = logger.entries();
        ASSERT_EQ(entries.size(), 2);
        EXPECT_EQ(entries[0].level, qga::LogLevel::Info);
        EXPECT_EQ(entries[1].level, qga::LogLevel::Err);
    }

    TEST_F(MockLoggerCaptureTest, GetLogsByLevelReturnsOnlyMatchingStrings)
    {
        logger.log(qga::LogLevel::Info, "Info Message A");
        logger.log(qga::LogLevel::Debug, "Debug Message");
        logger.log(qga::LogLevel::Info, "Info Message B");

        auto infoLogs = logger.getLogsByLevel(qga::LogLevel::Info);

        ASSERT_EQ(infoLogs.size(), 2);
        EXPECT_EQ(infoLogs[0], "Info Message A");
        EXPECT_EQ(infoLogs[1], "Info Message B");
    }

} // namespace qga::tests::fixtures::test
