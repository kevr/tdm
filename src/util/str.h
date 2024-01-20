#ifndef UTIL_STRING_H
#define UTIL_STRING_H

#include <cstring>
#include <string>
#include <vector>

namespace tdm {
std::string &lstrip(std::string &str);
std::string &rstrip(std::string &str);
std::string &strip(std::string &str);
std::vector<std::string> split(const std::string &str,
                               const std::string &delimiter,
                               std::size_t max_splits = 0);
};

#endif /* UTIL_STRING_H */
