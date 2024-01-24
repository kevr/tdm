#ifndef UTIL_FILESYSTEM_H
#define UTIL_FILESYSTEM_H

#include <filesystem>
#include <string>
#include <vector>

namespace tdm {
std::vector<std::filesystem::path>
listdir(const std::string &dir, const std::string &suffix = std::string());
};

#endif /* UTIL_FILESYSTEM_H */
