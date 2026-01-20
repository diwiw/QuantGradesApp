/**
 * @file grades_demo.cpp
 * @brief Legacy demo showing basic use of Grades, FileManager, and Config
 *        with the new asynchronous logging interface.
 */

#include <iostream>

#include "Grades.hpp"
#include "Version.hpp"
#include "core/Bootstrap.hpp"
#include "io/FileManager.hpp"
#include "utils/LoggerFactory.hpp"

using namespace qga::core;

/**
 * @brief Entry point for the Grades demo.
 *
 * Demonstrates usage of Grades, Config, and FileManager with
 * asynchronous logging via the new LoggerFactory system.
 */
int main()
{
    // === Header ===
    std::cout << "===================================\n";
    std::cout << " QuantGradesApp Config_Grades\n";
    std::cout << " Version: " << APP_VERSION << "\n";
    std::cout << " Build date: " << APP_BUILD_DATE << "\n";
    std::cout << "===================================\n\n";

    // === Bootstrap ===
    auto ctxOpt = bootstrapRuntime(std::filesystem::current_path(), AssetScope::GradesDemo);

    if (!ctxOpt)
    {
        std::cerr << "[FATAL] Runtime bootstrap failed\n";
        return 1;
    }

    auto& ctx = *ctxOpt;
    auto& cfg = ctx.cfg;

    // === Initialize logger===
    auto logger = qga::utils::LoggerFactory::createLogger(
        "GradesDemo", ctx.logDir / ctx.cfg.logFile().filename(), ctx.cfg.logLevel());

    logger->info("[APP] Started (cfg={})", ctx.configPath.string());

    // === Log warnings ===
    for (const auto& w : ctx.warnings)
        logger->warn("[Config] {}", w);

    std::cout << "dataDir=" << cfg.dataDir().string() << " threads=" << cfg.threads()
              << " logLevel=" << qga::toString(cfg.logLevel())
              << " logFile=" << (ctx.logDir / cfg.logFile().filename()).string() << '\n';

    // === Grades demo ===
    qga::core::Grades g;
    logger->info("[GradesDemo] Adding notes: 5, 4, 3");
    std::cout << "Adding notes: 5, 4, 3\n";
    g.add(5);
    g.add(4);
    g.add(3);

    g.printGrades();
    g.printSummary();

    // === Save grades ===
    const auto gradesOut = ctx.dataDir / "grades.txt";

    std::vector<std::string> grade_lines;
    for (int note : g.getNotes())
    {
        grade_lines.push_back(std::to_string(note));
    }

    if (!qga::io::FileManager::writeAllLines(gradesOut, grade_lines))
    {
        logger->error("[FileManager] Failed to write {}", gradesOut.string());
        return 1;
    }

    logger->info("[FileManager] Wrote {}", gradesOut.string());

    // === Read file ===
    const auto readPath = *ctx.assetsDir / "readGrades.txt";
    logger->info("[FileManager] Reading assets file {}", readPath.string());

    auto lines_opt = qga::io::FileManager::readAllLines(readPath);
    if (!lines_opt.has_value())
    {
        logger->error("[FileManager] Failed to read {}", readPath.string());
        std::cerr << "Error: Could not read file: " << readPath << "\n";
        return 1;
    }

    const auto& lines = lines_opt.value();
    if (lines.empty())
    {
        logger->warn("[FileManager] Empty file {}", readPath.string());
        return 0;
    }

    // === Display file ===
    std::cout << "\n=== File content: " << readPath << " ===\n";
    for (const auto& line : *lines_opt)
        std::cout << line << "\n";

    logger->info("[FileManager] File read successfully: {}", readPath.string());
    logger->info("[APP] Application finished successfully.");

    return 0;
}
