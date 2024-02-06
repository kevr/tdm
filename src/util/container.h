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

} // namespace tdm

#endif /* UTIL_CONTAINER_H */
