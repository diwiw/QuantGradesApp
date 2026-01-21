#pragma once
#include "core/IAppCLI.hpp"

/**
 * NOTE (bootstrap):
 * This CLI implementation directly orchestrates domain, ingest and IO layers.
 * This is intentional for 1.0.0 bootstrap and will be refactored into
 * an application service layer in milestone 1.1/1.0.5.
 */

namespace qga::cli
{

    /**
     * @brief CLI implementation of the QuantGradesApp application.
     *
     * This class implements the IAppCLI interface using the CLI11 library.
     * It provides the command-line interface used to run backtests,
     * validate input data, and export results.
     *
     * In future versions, this implementation may be replaced or extended
     * with Boost.Program_options or REST-based control.
     */
    class AppCLI : public qga::core::IAppCLI
    {
      public:
        /**
         * @brief Default constructor.
         *
         * Initializes the CLI application instance.
         * No heavy initialization is performed here — the actual setup
         * (such as logger creation and configuration loading)
         * happens during the `run()` execution.
         */
        AppCLI();

        /**
         * @brief Virtual destructor.
         *
         * Cleans up CLI-related resources if any were allocated.
         * Currently trivial, but declared explicitly to ensure proper
         * cleanup in case of subclassing or interface-based destruction.
         */
        ~AppCLI() override;

        /**
         * @brief Runs the CLI logic.
         *
         * Parses command-line arguments and triggers the corresponding
         * actions such as loading configuration, running a backtest,
         * or exporting results.
         *
         * @param argc Number of arguments.
         * @param argv Argument values.
         * @return Exit code (0 = success).
         */
        int run(int argc, char** argv) override;
    };

} // namespace qga::cli
