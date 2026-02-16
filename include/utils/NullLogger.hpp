/**
 * @file NullLogger.hpp
 * @brief Implementation of ILogger that discards all log messages (Null Object Pattern).
 *
 * This logger is useful in contexts where logging is optional or should be disabled.
 * It conforms to the ILogger interface but performs no actions.
 */

#pragma once

#include "utils/ILogger.hpp"

namespace qga::utils
{

    /**
     * @class NullLogger
     * @brief A no-op logger implementing the ILogger interface.
     *
     * Follows the Null Object Pattern. Can be injected anywhere an ILogger is required,
     * but will silently discard all log calls.
     */
    class NullLogger : public ILogger
    {
      public:
        /// @brief Does nothing.
        void log(qga::LogLevel /*level*/, const std::string& /*msg*/) override {}
    };

} // namespace qga::utils
