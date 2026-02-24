#pragma once

#include <gtest/gtest.h>

#include <filesystem>
#include <string>
#include <vector>

namespace qga::tests::fixtures
{
    /**
     * @brief Base test fixture that automatically cleans up tracked files after each test.
     */
    class BaseTestFixture : public ::testing::Test
    {
      protected:
        std::vector<std::string> filesToCleanUp;

        /// @brief Registers a file or directory to be deleted after the test finishes.
        void trackFile(const std::string& filepath) { filesToCleanUp.push_back(filepath); }

        /// @brief Automatically called by GTest after the test body finishes (even if it fails).
        void TearDown() override
        {
            for (const auto& file : filesToCleanUp)
            {
                if (std::filesystem::exists(file))
                {
                    // remove_all handles both files and entire directories
                    std::filesystem::remove_all(file);
                }
            }
        }
    };

} // namespace qga::tests::fixtures
