// SPDX-License-Identifier: MIT
#ifndef UTIL_FILESYSTEM_H
#define UTIL_FILESYSTEM_H

#include <filesystem>
#include <string>
#include <vector>

namespace tdm {

std::vector<std::filesystem::path>
listdir(const std::string &dir, const std::string &suffix = std::string());

/**
 * Search $PATH for filename
 *
 * @param filename Basename of file to search $PATH for, e.g. "echo"
 * @param found Set to true if filename was found in $PATH, otherwise false
 * @returns Full path to located file
 **/
std::filesystem::path search_path(const std::string &filename, bool &found);

/**
 * Search $PATH for filename
 *
 * An abstraction over search_path(filename, found) which throws a
 * std::runtime_error if found is false.
 *
 * @param filename Basename of file to search $PATH for, e.g. "echo"
 * @returns Full path to located file
 * @throws std::runtime_error If filename could not be found in $PATH
 **/
std::filesystem::path search_path(const std::string &filename);

// End of defs
} // namespace tdm

#endif /* UTIL_FILESYSTEM_H */
