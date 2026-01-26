#include <filesystem>
#include <string>
#include <vector>

#include "doctest.h"
#include "io/FileManager.hpp"
#include "unit_config.hpp"

namespace fs = std::filesystem;

static fs::path testFilePath()
{
    fs::path p = fs::path(QGA_UNIT_TEST_DIR) / "test_file.txt";
    if (fs::exists(p))
        fs::remove(p);
    return p;
}

TEST_CASE("FileManager basic file operations")
{
    fs::path test_file = testFilePath();

    SUBCASE("File does not exist at start")
    {
        CHECK_FALSE(qga::io::FileManager::exists(test_file.string()));
    }

    SUBCASE("WriteAllLines creates and writes file")
    {
        std::vector<std::string> lines = {"Line 1", "Line 2", "Line 3"};
        REQUIRE(qga::io::FileManager::writeAllLines(test_file.string(), lines));
        CHECK(qga::io::FileManager::exists(test_file.string()));

        auto read_lines = qga::io::FileManager::readAllLines(test_file.string());
        REQUIRE(read_lines.has_value());
        CHECK(read_lines->size() == 3);
        CHECK((*read_lines)[0] == "Line 1");
        CHECK((*read_lines)[1] == "Line 2");
        CHECK((*read_lines)[2] == "Line 3");
    }

    SUBCASE("AppendLine adds a new line at the end")
    {
        std::vector<std::string> lines = {"First"};
        REQUIRE(qga::io::FileManager::writeAllLines(test_file.string(), lines));

        REQUIRE(qga::io::FileManager::appendLine(test_file.string(), "Second"));
        auto read_lines = qga::io::FileManager::readAllLines(test_file.string());
        REQUIRE(read_lines.has_value());
        CHECK(read_lines->size() == 2);
        CHECK((*read_lines)[1] == "Second");
    }

    SUBCASE("RemoveFile deletes existing file")
    {
        std::vector<std::string> lines = {"Temp"};
        REQUIRE(qga::io::FileManager::writeAllLines(test_file.string(), lines));

        REQUIRE(qga::io::FileManager::removeFile(test_file.string()));
        CHECK_FALSE(qga::io::FileManager::exists(test_file.string()));
    }
}
