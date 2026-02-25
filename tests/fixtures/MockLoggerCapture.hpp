/**
 * @file MockLoggerCapture.hpp
 * @brief Test fixture logger that captures log entries in memory.
 *
 * This class is a test-only implementation of qga::utils::ILogger.
 * It records all log calls (above a configurable minimum level) into an in-memory buffer.
 *
 * Typical usage:
 *  - Unit tests: verify that a component produced expected log messages (spy/capture).
 *  - Integration tests: inspect log output without requiring spdlog sinks/files.
 *
 * Notes:
 *  - Thread-safe: suitable for concurrent tests where multiple threads may log.
 *  - Test-only: located under tests/fixtures and must not be used by production targets.
 */

#pragma once

#include <mutex>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "utils/ILogger.hpp"

namespace qga::tests::fixtures
{

    /**
     * @class MockLoggerCapture
     * @brief Captures log messages for inspection in tests.
     *
     * Provides helper methods to query recorded entries:
     *  - entries(): snapshot of all captured entries
     *  - contains(level, needle): checks if an entry contains a substring
     *  - count(level): number of entries at a given level
     *  - last(): last captured entry (if any)
     */
    class MockLoggerCapture : public qga::utils::ILogger
    {
      public:
        struct Entry
        {
            LogLevel level;
            std::string msg;
        };

        /// @brief Default constructor.
        MockLoggerCapture() = default;

        /// @brief Destructor.
        ~MockLoggerCapture() override = default;

        /// @brief Sets the minimum level for which logs are stored.
        void setLevel(qga::LogLevel level)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            minLevel_ = level;
        }

        /// @brief Clears all captured log entries.
        void clear()
        {
            std::lock_guard<std::mutex> lock(mutex_);
            entries_.clear();
        }

        /// @brief Returns a snapshot of all captured log entries.
        std::vector<Entry> entries() const
        {
            std::lock_guard<std::mutex> lock(mutex_);
            return entries_;
        }

        /// @brief Checks if a log entry contains a specific substring.
        /// @param level The log level to check.
        /// @param needle The substring to search for.
        /// @return True if a matching entry is found, false otherwise.
        bool contains(qga::LogLevel level, std::string_view needle) const
        {
            std::lock_guard<std::mutex> lock(mutex_);
            for (const auto& entry : entries_)
            {
                if (entry.level == level && entry.msg.find(needle) != std::string::npos)
                {
                    return true;
                }
            }
            return false;
        }

        /// @brief Counts the number of log entries at a specific level.
        /// @param level The log level to count.
        /// @return The number of log entries at the specified level.
        size_t count(qga::LogLevel level) const
        {
            std::lock_guard<std::mutex> lock(mutex_);
            size_t count = 0;
            for (const auto& entry : entries_)
            {
                if (entry.level == level)
                {
                    ++count;
                }
            }
            return count;
        }

        /// @brief Returns the last captured log entry (if any).
        std::optional<Entry> last() const
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (entries_.empty())
                return std::nullopt;
            return entries_.back();
        }

        /// @brief Returns all captured message strings (order preserved).
        std::vector<std::string> allLogs() const
        {
            std::lock_guard<std::mutex> lock(mutex_);
            std::vector<std::string> logs;
            logs.reserve(entries_.size());
            for (const auto& entry : entries_)
            {
                logs.push_back(entry.msg);
            }
            return logs;
        }

        /// @brief Returns all captured message strings (order preserved) for a specific log level.
        std::vector<std::string> getLogsByLevel(qga::LogLevel level) const
        {
            std::lock_guard<std::mutex> lock(mutex_);
            std::vector<std::string> logs;
            for (const auto& entry : entries_)
            {
                if (entry.level == level)
                {
                    logs.push_back(entry.msg);
                }
            }
            return logs;
        }

        /// @brief Logs a message at a specific severity level.
        void log(qga::LogLevel level, const std::string& msg) override
        {
            if (static_cast<int>(level) < static_cast<int>(minLevel_))
                return;

            std::lock_guard<std::mutex> lock(mutex_);
            entries_.push_back(Entry{level, msg});
        }

      private:
        mutable std::mutex mutex_;
        qga::LogLevel minLevel_ = qga::LogLevel::Debug;
        std::vector<Entry> entries_;
    };

} // namespace qga::tests::fixtures
