#include "filesystem.h"
#include <algorithm>

using std::filesystem::directory_iterator;

namespace tdm {
std::vector<std::filesystem::path> listdir(const std::string &dir,
                                           const std::string &suffix)
{
    std::vector<std::filesystem::path> files;

    auto suffix_len = suffix.size();
    for (const auto &entry : directory_iterator(dir)) {
        auto path = entry.path().string();
        if (path.rfind(suffix) == path.size() - suffix_len) {
            files.emplace_back(std::move(path));
        }
    }

    using value_type = decltype(files)::value_type;
    std::sort(files.begin(), files.end(), std::greater<value_type>());

    return files;
} // LCOV_EXCL_LINE
};
