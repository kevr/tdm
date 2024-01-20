#ifndef UTIL_ARGPARSE_H
#define UTIL_ARGPARSE_H

#include "termio.h"
#include <getopt.h>
#include <map>
#include <string>
#include <vector>

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
  public:
    static constexpr const char *usage = "usage: tdm [options]";

  private:
    struct option *m_options;

    // Parsed values
    std::map<std::string, std::string> m_values;

    // Optional help descriptions
    std::vector<std::pair<std::string, std::string>> m_desc;

  public:
    Args(struct option *options);

  public:
    //! Parse command-line arguments
    int parse(int argc, const char **argv);

    //! Check if an option was parsed
    bool has(const std::string &opt) const;

    //! Get an option's value
    template <typename T = std::string>
    T get(const std::string &opt) const
    {
        return Convert<T>::cast(m_values.at(opt));
    }

    //! Describe a long option
    Args &describe(std::string option_name, std::string desc);

    //! Describe a long & short option
    Args &describe(std::string option_name, char short_name, std::string desc);

    //! Return a help string containing described options
    std::string help(void) const;

  private:
    void reset_getopt(void);

    Args &describe_formatted(std::string option, std::string desc);
};

}; // namespace tdm

#endif /* UTIL_ARGPARSE_H */
