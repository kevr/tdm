// SPDX-License-Identifier: MIT
#include "filesystem.h"
#include "../lib/sys.h"
#include "logger.h"
#include "str.h"
#include <algorithm>
#include <sys/stat.h>

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

int makedir(const std::filesystem::path &path)
{
    try {
        if (!lib::sys->exists(path)) {
            if (lib::sys->mkdir(path.c_str(), 0755) == -1) {
                logger.error(R"(failed to create directory "{}")",
                             path.c_str());
                logger.debug(R"(mkdir("{}") returned -1)", path.c_str());
                return -1;
            } else {
                logger.debug(R"(mkdir("{}"))", path.c_str());
            }
        }
    } catch (std::exception &) {
        logger.error(R"(unable to open "{}" for reading)", path.c_str());
        return -1;
    }

    return 0;
}

int makedirs(const std::filesystem::path &path)
{
    auto segments = split(path.string(), "/");
    std::filesystem::path current("/");
    for (auto &seg : segments) {
        current /= seg;
        if (int e = makedir(current); e != 0) {
            return e;
        }
    }
    return 0;
}

} // namespace tdm
