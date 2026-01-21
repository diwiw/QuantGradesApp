#include "core/AssetsLocator.hpp"

namespace fs = std::filesystem;

namespace qga::core
{
    static fs::path scopeToRelativePath(AssetScope scope)
    {
        switch (scope)
        {
        case AssetScope::GradesDemo:
            return "examples/grades_demo/assets";
        case AssetScope::LoggerDemo:
            return "examples/logger_demo/assets";
        case AssetScope::BacktestDemo:
            return "examples/backtest_demo/assets";
        }
        return {};
    }

    std::optional<fs::path> findAssetsDir(AssetScope scope, const fs::path& startDir)
    {
        fs::path dir = fs::absolute(startDir);
        const auto rel = scopeToRelativePath(scope);

        while (!dir.empty())
        {
            const fs::path candidate = dir / rel;
            if (fs::exists(candidate) && fs::is_directory(candidate))
                return candidate;

            if (!dir.has_parent_path())
                break;

            dir = dir.parent_path();
        }
        return std::nullopt;
    }
} // namespace qga::core