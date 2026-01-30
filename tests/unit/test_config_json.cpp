#include <filesystem>
#include <fstream>
#include <thread>

#include "common/LogLevel.hpp"
#include "core/Config.hpp"
#include "doctest.h"
#include "unit_config.hpp"

// --- FIX FOR WINDOWS START ---
#ifdef _WIN32
#include <string>

//  _putenv_s
int setenv(const char* name, const char* value, int overwrite)
{
    // On Windows _putenv_s returns 0 on success
    return _putenv_s(name, value);
}

// Windows does not have unsetenv, set the variable to an empty string
int unsetenv(const char* name) { return _putenv_s(name, ""); }
#endif
// --- FIX FOR WINDOWS END ---

namespace fs = std::filesystem;
using namespace qga::core;

// Add doctest stringifier for qga::LogLevel
namespace doctest
{
    template <> struct StringMaker<qga::LogLevel>
    {
        static String convert(const qga::LogLevel& lvl) { return String(qga::toString(lvl)); }
    };
} // namespace doctest

static fs::path tmpdir()
{
    fs::path d = fs::path(QGA_UNIT_TEST_DIR) / "test_tmp_cfg_json";
    fs::create_directories(d);
    return d;
}

TEST_SUITE("Config/JSON")
{
    TEST_CASE("Load full JSON and sets fields" * doctest::skip())
    {
#if 0
        fs::path d = tmpdir();
        auto f = d / "full.json";
        std::ofstream out(f);
        out << R"({
            "logging": {"level": "DEBUG", "file": "logs/test_log.txt" },
            "paths": { "data_dir": "dataset" },
            "engine": { "threads": 8 }
        })";
        out.close();

        auto& cfg = reset_cfg();
        cfg.loadFromFile(f);

        CHECK(qga::toString(cfg.logLevel()) == qga::toString(qga::LogLevel::Debug));
        CHECK(cfg.logFile() == fs::path("logs/test_log.txt"));
        CHECK(cfg.dataDir() == fs::path("dataset"));
        CHECK(cfg.threads() == 8);
#endif
    }

    TEST_CASE("Missing keys keep defaults" * doctest::skip())
    {
#if 0
        auto d = tmpdir();
        auto f = d / "missing_keys.json";
        std::ofstream out(f);
        out << R"({
            "paths": { "data_dir": "data2" }
        })";
        out.close();

        auto& cfg = reset_cfg();
        cfg.loadFromFile(f);

        CHECK(qga::toString(cfg.logLevel()) == qga::toString(qga::LogLevel::Info));
        CHECK(cfg.logFile() == fs::path("logs/qga.log")); // Default value
        CHECK(cfg.dataDir() == fs::path("data2"));
        CHECK(cfg.threads() == 4); // Default value
#endif
    }

    TEST_CASE("Invalid threads fallback to safe value" * doctest::skip())
    {
#if 0
        auto d = tmpdir();
        auto f = d / "bad_threads.json";
        std::ofstream out(f);
        out << R"({ "engine": { "threads": -5 } })";
        out.close();

        auto& cfg = reset_cfg();
        std::vector<std::string> warnings;
        cfg.loadFromFile(f, &warnings);

        CHECK(cfg.threads() == 1);
        CHECK(qga::toString(cfg.logLevel()) == qga::toString(qga::LogLevel::Info));
        CHECK(warnings[0].find("engine.threads < 1") != std::string::npos);
#endif
    }

    TEST_CASE("Invalid log level falls back to INFO" * doctest::skip())
    {
#if 0
        auto d = tmpdir();
        auto f = d / "bad_loglevel.json";
        std::ofstream out(f);
        out << R"({ "logging": { "level": "VERBOSE" } })";
        out.close();

        auto& cfg = reset_cfg();
        std::vector<std::string> warnings;
        cfg.loadFromFile(f, &warnings);

        CHECK(qga::toString(cfg.logLevel()) == qga::toString(qga::LogLevel::Info));
        CHECK(warnings[0].find("logging.level: unknown value") != std::string::npos);
#endif
    }

    TEST_CASE("Malformed JSON keeps defaults and adds warning")
    {
        auto d = tmpdir();
        auto f = d / "malformed.json";
        std::ofstream out(f);
        out << R"({ "logging": { "level": "DEBUG", )"; // incomplete JSON
        out.close();

        auto& cfg = Config::getInstance();
        cfg.loadDefaults();
        std::vector<std::string> warnings;
        cfg.loadFromFile(f, &warnings);

        // Defaults should remain
        CHECK(qga::toString(cfg.logLevel()) == qga::toString(qga::LogLevel::Info));
        CHECK(cfg.logFile() == fs::path("logs/qga.log"));
        CHECK(cfg.dataDir() == fs::path("data"));
        CHECK(cfg.threads() == 4);

        CHECK(warnings[0].find("Invalid JSON") != std::string::npos);
    }

    TEST_CASE("Threads above hardware concurrency capped" * doctest::skip())
    {
#if 0
        auto d = tmpdir();
        auto f = d / "too_many_threads.json";
        std::ofstream out(f);
        out << R"({ "engine": { "threads": 9999 } })";
        out.close();

        auto& cfg = reset_cfg();
        std::vector<std::string> warnings;
        cfg.loadFromFile(f, &warnings);

        unsigned hw = std::max(1u, std::thread::hardware_concurrency());
        CHECK(cfg.threads() == static_cast<int>(hw));
        CHECK(qga::toString(cfg.logLevel()) == qga::toString(qga::LogLevel::Info));
        CHECK(warnings[0].find("engine.threads > CPU cores") != std::string::npos);
#endif
    }

    TEST_CASE("Empty string values fall back to defaults" * doctest::skip())
    {
#if 0
        auto d = tmpdir();
        auto f = d / "empty_strings.json";
        std::ofstream out(f);
        out << R"({
            "paths": { "data_dir": "" },
            "logging": { "file": "" }
        })";
        out.close();

        auto& cfg = reset_cfg();
        std::vector<std::string> warnings;
        cfg.loadFromFile(f, &warnings);

        CHECK(cfg.dataDir() == fs::path("data"));    // fallback
        CHECK(cfg.logFile() == fs::path("app.log")); // fallback
        REQUIRE(warnings.size() >= 2);
        CHECK_MESSAGE((warnings[0].find("paths.data_dir is empty") != std::string::npos
                       || warnings[1].find("paths.data_dir is empty") != std::string::npos),
                      "Expected warning about empty data_dir");

        CHECK_MESSAGE((warnings[0].find("logging.file is empty") != std::string::npos
                       || warnings[1].find("logging.file is empty") != std::string::npos),
                      "Expected warning about empty log file");
#endif
    }

    TEST_CASE("Missing config file keeps defaults and warns" * doctest::skip())
    {
#if 0
        auto d = tmpdir();
        auto f = d / "nonexistent.json"; // do not create this file

        auto& cfg = reset_cfg();
        std::vector<std::string> warnings;
        cfg.loadFromFile(f, &warnings);

        CHECK(cfg.dataDir() == fs::path("data"));
        CHECK(cfg.logFile() == fs::path("logs/qga.log"));
        CHECK(cfg.threads() == 4);
        CHECK(qga::toString(cfg.logLevel()) == qga::toString(qga::LogLevel::Info));
        CHECK(warnings[0].find("Config file not found") != std::string::npos);
#endif
    }
}

TEST_SUITE("Config/Profiles")
{
    // clean env between tests
    auto unset_all = []()
    {
        unsetenv("QGA_PROFILE");
        unsetenv("QGA_DATA_DIR");
        unsetenv("QGA_THREADS");
        unsetenv("QGA_LOG_LEVEL");
        unsetenv("QGA_LOG_FILE");
    };

    TEST_CASE("Default profile is prod (profile == empty) when QGA_PROFILE is missing")
    {
        unset_all();

        auto& cfg = Config::getInstance();
        cfg.loadDefaults();
        cfg.loadFromEnv();

        CHECK(cfg.profile().empty());
    }

    TEST_CASE("Loads correct profile JSON: test")
    {
        unset_all();

        // prepare test profile config
        fs::create_directories("config");                 // real layout
        auto f = fs::path("config") / "config.test.json"; // real path

        std::ofstream out(f);
        out << R"({
            "logging": { "level": "WARN" },
            "paths":   { "data_dir": "test_data" },
            "engine":  { "threads": 2 }
        })";
        out.close();

        // point working dir so Config can find config/config.test.json
        // (we copy generated file into cwd)
        fs::copy(f, "config.test.json", fs::copy_options::overwrite_existing);

        setenv("QGA_PROFILE", "test", 1);

        auto& cfg = Config::getInstance();
        cfg.loadDefaults();
        std::vector<std::string> warnings;
        cfg.loadFromEnv(&warnings);

        CHECK(cfg.profile() == "test");
        CHECK(cfg.dataDir() == fs::path("test_data"));
        CHECK(cfg.threads() == 2);
        CHECK(qga::toString(cfg.logLevel()) == qga::toString(qga::LogLevel::Warn));
    }

    TEST_CASE("Loads correct profile JSON: prod")
    {
        unset_all();

        fs::create_directories("config");
        auto f = fs::path("config") / "config.prod.json";

        std::ofstream out(f);
        out << R"({
            "logging": { "level": "INFO" },
            "paths":   { "data_dir": "live_data" },
            "engine":  { "threads": 1 }
        })";
        out.close();

        setenv("QGA_PROFILE", "prod", 1);

        auto& cfg = Config::getInstance();
        cfg.loadDefaults();
        std::vector<std::string> warnings;
        cfg.loadFromEnv(&warnings);

        CHECK(cfg.profile() == "prod");
        CHECK(cfg.dataDir() == fs::path("live_data"));
        CHECK(cfg.threads() == 1);
    }

    TEST_CASE("Env overrides profile JSON values (but not profile)" * doctest::skip())
    {

        unset_all();

        // config.dev.json
        fs::create_directories("config");
        auto f = fs::path("config") / "config.dev.json";

        std::ofstream out(f);
        out << R"({
            "logging": { "level": "DEBUG" },
            "paths":   { "data_dir": "dev_data" }
        })";
        out.close();

        unsetenv("QGA_PROFILE"); // default => dev
        setenv("QGA_DATA_DIR", "override_data", 1);
        setenv("QGA_THREADS", "99", 1);

        auto& cfg = Config::getInstance();
        cfg.loadDefaults();
        cfg.loadFromEnv();

        CHECK(cfg.profile() == "dev");                     // profile NOT overridden by env
        CHECK(cfg.dataDir() == fs::path("override_data")); // ENV wins
        unsigned hw = std::max(1u, std::thread::hardware_concurrency());
        CHECK(cfg.threads() == static_cast<int>(hw)); // ENV wins
    }

    TEST_CASE("Missing profile JSON produces warning and keeps defaults")
    {
        unset_all();
        setenv("QGA_PROFILE", "nonexistentProfile123", 1);

        auto& cfg = Config::getInstance();
        cfg.loadDefaults();
        std::vector<std::string> warnings;
        cfg.loadFromEnv(&warnings);

        CHECK(cfg.profile() == "nonexistentProfile123"); // still the selected profile
        CHECK(cfg.dataDir() == fs::path("data"));
        CHECK(cfg.logFile() == fs::path("logs/qga.log"));
        CHECK(cfg.threads() == 4);

        CHECK(qga::toString(cfg.logLevel()) == qga::toString(qga::LogLevel::Info));
        CHECK(warnings[0].find("Config file not found") != std::string::npos);
    }
}
