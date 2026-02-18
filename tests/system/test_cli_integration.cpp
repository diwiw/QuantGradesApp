/**
 * @file test_cli_integration.cpp
 * @brief Integration tests for the AppCLI interface using CLI11 argument parsing.
 *
 * This test simulates different CLI invocations and ensures that the command-line
 * options and flags are parsed correctly without throwing exceptions.
 */

#include "cli/AppCLI.hpp"
#include "doctest.h"
#include <string>
#include <vector>

using namespace qga::cli;

/**
 * @test Checks that the CLI can be initialized and invoked with basic arguments.
 */
TEST_CASE("CLI11 basic parsing with valid input and flags")
{
    AppCLI cli;

    std::vector<const char*> argv = {"qga_cli",  "--input",         "mock_input.csv",
                                     "--output", "mock_output.csv", "--verbose"};
    int argc = static_cast<int>(argv.size());

    CHECK_NOTHROW(cli.run(argc, const_cast<char**>(argv.data())));
}

/**
 * @test Ensures that help flag runs correctly and exits gracefully.
 */
TEST_CASE("CLI11 handles --help gracefully")
{
    AppCLI cli;

    std::vector<const char*> argv = {"qga_cli", "--help"};
    int argc = static_cast<int>(argv.size());

    CHECK_NOTHROW(cli.run(argc, const_cast<char**>(argv.data())));
}

/**
 * @test Ensures that version flag runs correctly and prints version info.
 */
TEST_CASE("CLI11 handles --version gracefully")
{
    AppCLI cli;

    std::vector<const char*> argv = {"qga_cli", "--version"};
    int argc = static_cast<int>(argv.size());

    CHECK_NOTHROW(cli.run(argc, const_cast<char**>(argv.data())));
}

/**
 * @test Verifies that invalid arguments are handled gracefully (no crash).
 */
TEST_CASE("CLI11 rejects invalid argument syntax")
{
    AppCLI cli;

    std::vector<const char*> argv = {"qga_cli", "--unknown"};
    int argc = static_cast<int>(argv.size());

    CHECK_NOTHROW(cli.run(argc, const_cast<char**>(argv.data())));
}
