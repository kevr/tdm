// SPDX-License-Identifier: MIT
#ifndef UTIL_CONTAINER_H
#define UTIL_CONTAINER_H

#include <map>
#include <memory>
#include <vector>

namespace tdm {

template <typename U, typename T>
std::vector<T> values(const std::map<U, T> &m)
{
    std::vector<T> v;
    for (auto &kv : m)
        v.emplace_back(kv.second);
    return v;
} // LCOV_EXCL_LINE

template <template <typename, typename> class Container, typename String,
          typename Alloc>
std::vector<char *> to_argv(const Container<String, Alloc> &container)
{
    std::vector<char *> argv;
    for (auto &entry : container)
        argv.emplace_back(const_cast<char *>(entry.data()));
    argv.emplace_back(nullptr);
    return argv;
} // LCOV_EXCL_LINE

} // namespace tdm

#endif /* UTIL_CONTAINER_H */
