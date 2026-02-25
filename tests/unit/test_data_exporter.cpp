#include <filesystem>
#include <fstream>
#include <sstream>

#include "doctest.h"
#include "domain/backtest/BarSeries.hpp"
#include "fixtures/MockLoggerCapture.hpp"
#include "io/DataExporter.hpp"

using namespace qga::io;
using namespace qga::domain::backtest;
using namespace qga::utils;
namespace fs = std::filesystem;

static fs::path tmpdir()
{
    fs::path d{"test_tmp_export"};
    fs::create_directories(d);
    return d;
}

static BarSeries sampleSeries()
{
    BarSeries s;
    s.add({1669900800000, 100.5, 102.3, 99.0, 101.2, 12345.67});
    s.add({1669987200000, 101.2, 103.0, 100.0, 102.5, 14500.00});
    return s;
}

TEST_SUITE("DataExporter")
{

    TEST_CASE("Export full series to CSV")
    {
        auto d = tmpdir();
        auto path = d / "out_full.csv";
        auto log = std::make_shared<qga::tests::fixtures::MockLoggerCapture>();

        DataExporter exporter(path.string(), log, ExportFormat::CSV, false);
        auto series = sampleSeries();
        exporter.exportSeries(series);

        // === Verify file content ===
        std::ifstream in(path);
        REQUIRE(in.is_open());

        std::string header;
        std::getline(in, header);
        CHECK(header == "timestamp,open,high,low,close,volume");

        std::string line;
        std::getline(in, line);
        std::stringstream ss(line);
        long long ts;
        double open, high, low, close, vol;
        char comma;
        ss >> ts >> comma >> open >> comma >> high >> comma >> low >> comma >> close >> comma
            >> vol;

        CHECK(ts == 1669900800000);
        CHECK(open == doctest::Approx(100.5));
        CHECK(high == doctest::Approx(102.3));
        CHECK(low == doctest::Approx(99.0));
        CHECK(close == doctest::Approx(101.2));
        CHECK(vol == doctest::Approx(12345.67));

        // === Verify logs ===
        auto logs = log->allLogs();
        REQUIRE_FALSE(logs.empty());
        bool found = false;
        for (auto& l : logs)
        {
            if (l.find("DataExporter: successfully exported") != std::string::npos)
            {
                found = true;
                break;
            }
        }
        CHECK(found);
    }

    TEST_CASE("Export range of series")
    {
        auto d = tmpdir();
        auto path = d / "out_range.csv";
        auto log = std::make_shared<qga::tests::fixtures::MockLoggerCapture>();

        DataExporter exporter(path.string(), log, ExportFormat::CSV, false);
        auto series = sampleSeries();
        exporter.exportRange(series, 0, 1);

        // === Verify file content ===
        std::ifstream in(path);
        REQUIRE(in.is_open());

        std::string header;
        std::getline(in, header);
        CHECK(header == "timestamp,open,high,low,close,volume");

        std::string line;
        std::getline(in, line);
        std::stringstream ss(line);
        long long ts;
        double open, high, low, close, vol;
        char comma;
        ss >> ts >> comma >> open >> comma >> high >> comma >> low >> comma >> close >> comma
            >> vol;

        CHECK(ts == 1669900800000);
        CHECK(open == doctest::Approx(100.5));
        CHECK(high == doctest::Approx(102.3));
        CHECK(low == doctest::Approx(99.0));
        CHECK(close == doctest::Approx(101.2));
        CHECK(vol == doctest::Approx(12345.67));

        // === Verify logs ===
        auto logs = log->allLogs();
        REQUIRE_FALSE(logs.empty());
        bool found = false;
        for (auto& l : logs)
        {
            if (l.find("DataExporter: exported") != std::string::npos)
            {
                found = true;
                break;
            }
        }
        CHECK(found);
    }

    TEST_CASE("Export empty series throws")
    {
        auto d = tmpdir();
        auto path = d / "out_empty.csv";
        auto log = std::make_shared<qga::tests::fixtures::MockLoggerCapture>();

        DataExporter exporter(path.string(), log, ExportFormat::CSV, false);
        BarSeries empty;

        CHECK_THROWS_AS(exporter.exportSeries(empty), std::invalid_argument);

        auto logs = log->allLogs();
        REQUIRE_FALSE(logs.empty());
        bool found = false;
        for (auto& l : logs)
        {
            if (l.find("DataExporter: cannot export empty BarSeries") != std::string::npos)
            {
                found = true;
                break;
            }
        }
        CHECK(found);
    }

    TEST_CASE("Export full series via exportAll logs correctly")
    {
        auto d = tmpdir();
        auto f = d / "all.csv";
        auto log = std::make_shared<qga::tests::fixtures::MockLoggerCapture>();

        BarSeries series;
        series.add({1669900800000, 100.5, 102.3, 99.0, 101.2, 12345.67});
        series.add({1669987200000, 101.2, 103.0, 100.0, 102.5, 14500.0});

        DataExporter exporter(f.string(), log, ExportFormat::CSV, false);
        exporter.exportAll(series);

        auto logs = log->allLogs();
        REQUIRE_FALSE(logs.empty());
        CHECK(logs.back().find("DataExporter: successfully exported") != std::string::npos);
    }
}
