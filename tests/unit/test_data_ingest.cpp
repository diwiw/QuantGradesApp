#include "doctest.h"
#include "fixtures/MockLoggerCapture.hpp"
#include "ingest/DataIngest.hpp"

namespace qga::ingest
{

    // Test helper with friend access
    class DataIngestTestAccess
    {
      public:
        static bool validateRow(DataIngest& ingest, const std::vector<std::string>& fields)
        {
            return ingest.validateRow(fields);
        }

        static std::optional<qga::domain::Quote> parseRow(DataIngest& ingest,
                                                          const std::vector<std::string>& fields)
        {
            return ingest.parseRow(fields);
        }
    };

    TEST_SUITE("DataIngest - unit tests")
    {

        TEST_CASE("validateRow returns true for valid field count")
        {
            std::vector<std::string> valid_row = {"1625097600000", "100.0", "105.0",
                                                  "99.0",          "104.0", "12345.67"};

            auto ingest = DataIngest(std::make_shared<qga::tests::fixtures::MockLoggerCapture>());
            CHECK(DataIngestTestAccess::validateRow(ingest, valid_row));
        }

        TEST_CASE("validateRow returns false for invalid field count")
        {
            std::vector<std::string> invalid_row = {
                "1625097600000", "100.0" // only 2 fields
            };

            auto ingest = DataIngest(std::make_shared<qga::tests::fixtures::MockLoggerCapture>());
            CHECK_FALSE(DataIngestTestAccess::validateRow(ingest, invalid_row));
        }

        TEST_CASE("parseRow returns Quote for valid fields")
        {
            std::vector<std::string> fields = {"1669900800000", "100.5", "102.3",
                                               "99.0",          "101.2", "12345.67"};

            auto ingest = DataIngest(std::make_shared<qga::tests::fixtures::MockLoggerCapture>());
            auto result = DataIngestTestAccess::parseRow(ingest, fields);

            REQUIRE(result.has_value());
            CHECK(result->ts_ == 1669900800000);
            CHECK(result->open_ == doctest::Approx(100.5));
            CHECK(result->high_ == doctest::Approx(102.3));
            CHECK(result->low_ == doctest::Approx(99.0));
            CHECK(result->close_ == doctest::Approx(101.2));
            CHECK(result->volume_ == doctest::Approx(12345.67));
        }
    }

} // namespace qga::ingest
