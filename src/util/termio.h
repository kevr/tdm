// SPDX-License-Identifier: MIT
#ifndef TERMIO_H
#define TERMIO_H

#include <fmt/format.h>
#include <iostream>
#include <string>

namespace tdm {

/*! Send a string to an std::ostream */
void print_s(std::ostream &os, const std::string &str);

/*! Print a formatted string to std::cout */
template <typename... Args>
int print(fmt::format_string<Args...> fmt, Args &&...args)
{
    print_s(std::cout, fmt::format(fmt, std::forward<Args>(args)...));
    return 0;
}

/*! Print a formatted string to std::cerr */
template <typename... Args>
int error(int rc, fmt::format_string<Args...> fmt, Args &&...args)
{
    print_s(std::cerr,
            fmt::format("error: {}",
                        fmt::format(fmt, std::forward<Args>(args)...)));
    return rc;
}

}; // namespace tdm

#endif /* TERMIO_H */
