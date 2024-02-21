// SPDX-License-Identifier: MIT
#ifndef UTIL_STRING_H
#define UTIL_STRING_H

#include <cstring>
#include <memory>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

namespace tdm {
std::string &lstrip(std::string &str);
std::string &rstrip(std::string &str);
std::string &strip(std::string &str);
std::vector<std::string> split(const std::string &str,
                               const std::string &delimiter,
                               std::size_t max_splits = 0);

template <typename T = std::string>
struct Join
{
    static std::string call(const std::vector<T> &vec, const std::string &sep)
    {
        std::string str;

        int i, size = vec.size();
        for (i = 0; i < size - 1; ++i) {
            str.append(vec[i]);
            str.append(sep);
        }

        if (i < size) {
            str.append(vec[i]);
        }

        return str;
    } // LCOV_EXCL_LINE
};

template <typename T>
std::string join(const std::vector<T> &vec, const std::string &sep)
{
    return Join<T>::call(vec, sep);
}

}; // namespace tdm

#endif /* UTIL_STRING_H */
