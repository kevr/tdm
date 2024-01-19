#ifndef TERMIO_H
#define TERMIO_H

#include <format>
#include <iostream>

namespace tdm {

/*! Send a string to an std::ostream */
void print_s(std::ostream &os, const std::string &str);

/*! Print a formatted string to std::cout */
template <typename... Args>
void print(std::format_string<Args...> fmt, Args &&...args)
{
    print_s(std::cout, std::format(fmt, std::forward<Args>(args)...));
}

/*! Print a formatted string to std::cerr */
template <typename... Args>
int error(int rc, std::format_string<Args...> fmt, Args &&...args)
{
    print_s(std::cerr,
            std::format("error: {}",
                        std::format(fmt, std::forward<Args>(args)...)));
    return rc;
}

}; // namespace tdm

#endif /* TERMIO_H */
