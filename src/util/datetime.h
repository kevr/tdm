// SPDX-License-Identifier: MIT
#ifndef UTIL_DATETIME_H
#define UTIL_DATETIME_H

#include <chrono>
#include <string>

namespace tdm::datetime::system {

std::chrono::system_clock::time_point now(void);
std::string strftime(const std::chrono::system_clock::time_point &tp,
                     const char *fmt);

} // namespace tdm::datetime::system

#endif /* UTIL_DATETIME_H */
