#pragma once

namespace qga::core
{

    /**
     * @brief Interface for the main QuantGradesApp CLI entry point.
     *
     * This interface defines the abstract entry point for user interaction
     * with the QuantGradesApp application via the command line.
     *
     * Different implementations can use various argument parsers (CLI11, Boost.Program_options,
     * etc.) or even represent other frontends like REST or GUI.
     */
    class IAppCLI
    {
      public:
        virtual ~IAppCLI() = default;

        /**
         * @brief Runs the CLI application.
         *
         * @param argc The number of command-line arguments.
         * @param argv The array of command-line arguments.
         * @return Exit code (0 = success).
         */
        virtual int run(int argc, char** argv) = 0;
    };

} // namespace qga::core
