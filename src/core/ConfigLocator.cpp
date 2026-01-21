#include "core/ConfigLocator.hpp"

namespace qga::core
{

    std::optional<std::filesystem::path> findConfigFile(const std::filesystem::path& startDir)
    {
        auto dir = std::filesystem::absolute(startDir);

        while (true)
        {
            auto candidate = dir / "config" / "config.json";
            if (std::filesystem::exists(candidate))
                return candidate;

            if (!dir.has_parent_path())
                break;

            dir = dir.parent_path();
        }

        return std::nullopt;
    }
} // namespace qga::core