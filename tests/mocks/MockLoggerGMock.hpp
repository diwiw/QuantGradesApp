#pragma once

#include <gmock/gmock.h>

#include "utils/ILogger.hpp"

namespace qga::tests::mocks
{

    class MockLoggerGMock : public utils::ILogger
    {
      public:
        MOCK_METHOD(void, log, (qga::LogLevel level, const std::string& msg), (override));

        MOCK_METHOD(void, flush, (), (noexcept, override));
    };

} // namespace qga::tests
