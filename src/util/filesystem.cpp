#include "filesystem.h"
#include "logger.h"
#include "str.h"
#include <algorithm>

using std::filesystem::directory_iterator;

namespace tdm {
std::vector<std::filesystem::path> listdir(const std::string &dir,
                                           const std::string &suffix)
{
    std::vector<std::filesystem::path> files;

    try {
        auto suffix_len = suffix.size();
        for (const auto &entry : directory_iterator(dir)) {
            auto path = entry.path().string();
            if (path.rfind(suffix) == path.size() - suffix_len) {
                files.emplace_back(std::move(path));
            }
        }
    } catch (std::exception &exc) {
        logger.error("{}", exc.what());
    }

    using value_type = decltype(files)::value_type;
    std::sort(files.begin(), files.end(), std::greater<value_type>());

    return files;
} // LCOV_EXCL_LINE

std::filesystem::path search_path(const std::string &filename, bool &found)
{
    auto search_dirs = split(getenv("PATH"), ":");

    for (auto &dir : search_dirs) {
        auto path_ = std::filesystem::path(dir) / filename;
        if (std::filesystem::exists(path_)) {
            found = true;
            return path_;
        }
    }

    found = false;
    return filename;
}

std::filesystem::path search_path(const std::string &filename)
{
    bool found;
    auto result = search_path(filename, found);
    if (!found) {
        std::string msg = fmt::format("unable to locate {} in $PATH", filename);
        throw std::domain_error(msg);
    }
    return result;
}

} // namespace tdm
