#include "api/ApiServer.hpp"
#include "core/Config.hpp"
#include "doctest.h"
#include "fixtures/MockLoggerCapture.hpp"

TEST_CASE("ApiServer unified error JSON")
{
    std::string json = qga::api::ApiServer::makeErrorJson("TEST_ERROR", "Something broke");

    CHECK(json.find("\"code\":\"TEST_ERROR\"") != std::string::npos);
    CHECK(json.find("\"message\":\"Something broke\"") != std::string::npos);
}

TEST_CASE("ApiServer constructs without throwing")
{
    auto logger = std::make_shared<qga::tests::fixtures::MockLoggerCapture>();
    auto& config = qga::core::Config::getInstance();

    // Reset defaults for test isolation
    config.loadDefaults();

    CHECK_NOTHROW(qga::api::ApiServer server(logger, config));
}

TEST_CASE("ApiServer logs on construction")
{
    auto logger = std::make_shared<qga::tests::fixtures::MockLoggerCapture>();
    auto& config = qga::core::Config::getInstance();

    config.loadDefaults();

    qga::api::ApiServer server(logger, config);

    auto logs = logger->allLogs();

    bool found_message = false;
    for (auto& msg : logs)
    {
        if (msg.find("API Server created") != std::string::npos)
        {
            found_message = true;
            break;
        }
    }

    CHECK(found_message);
}
