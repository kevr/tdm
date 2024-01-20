#ifndef UTIL_ARGPARSE_H
#define UTIL_ARGPARSE_H

#include "termio.h"
#include <getopt.h>
#include <map>
#include <string>

#define OPT_LONG -2

namespace tdm {

template <typename T = std::string>
struct Convert
{
    static T cast(const std::string &str) { return str; }
};

template <>
struct Convert<int>
{
    static int cast(const std::string &str) { return std::stoi(str); }
};

class Args
{
  private:
    struct option *m_options;
    std::map<std::string, std::string> m_values;

  public:
    Args(struct option *options);

  public:
    //! Parse command-line arguments
    int parse(int argc, char **argv);

    //! Check if an option was parsed
    bool has(const std::string &opt) const;

    //! Get an option's value
    template <typename T = std::string>
    T get(const std::string &opt) const
    {
        return Convert<T>::cast(m_values.at(opt));
    }
};

}; // namespace tdm

#endif /* UTIL_ARGPARSE_H */
