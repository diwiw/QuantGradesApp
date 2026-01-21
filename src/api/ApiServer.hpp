/**
 * @file ApiServer.hpp
 * @brief Lightweight REST API server for QuantGradesApp.
 *
 * Responsibilities:
 *  - Registering and serving HTTP endpoints
 *  - Returning unified JSON responses
 *  - Integrating with Config + Logger
 *
 * API Endpoints (v1):
 *  GET  /health
 *  GET  /version
 *  POST /ingest/csv
 *  GET  /grades/report
 */

#pragma once
#include <memory>
#include <string>

#include "core/Config.hpp"
#include "httplib.h"
#include "utils/ILogger.hpp"

namespace qga::api
{

    class ApiServer
    {
      public:
        /// @brief Construct server with injected logger + config reference
        ApiServer(std::shared_ptr<utils::ILogger> logger, const qga::core::Config& config);

        /// @brief Start blocking HTTP loop
        void start();

        /// @brief Stop server gracefully
        void stop();

        /// @brief Generate unified JSON error envelope
        static std::string makeErrorJson(const std::string& code, const std::string& message);

      private:
        void registerEndpoints();

      private:
        std::shared_ptr<utils::ILogger> logger_;
        const qga::core::Config& config_; // IMPORTANT: reference, not a copy
        httplib::Server server_;
    };

} // namespace qga::api
