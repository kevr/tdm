#ifndef UTIL_ARGPARSE_H
#define UTIL_ARGPARSE_H

#include "../config.h"
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
    static T cast(const std::string &str)
    {
        return str;
    }
};

template <>
struct Convert<int>
{
    static int cast(const std::string &str)
    {
        return std::stoi(str);
    }
};

class Args
{
  public:
    static constexpr const char *usage = "usage: " PROJECT_NAME " [options]";

  private:
    struct option *m_options;

    // Parsed values
    std::map<std::string, std::string> m_values;

    // Default option values
    std::map<std::string, std::string> m_defaults;

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
    const T &get(const std::string &opt) const
    {
        auto it = m_values.find(opt);
        if (it != m_values.end()) {
            return Convert<const T &>::cast(it->second);
        }
        return Convert<const T &>::cast(m_defaults.at(opt));
    }

    //! Describe a long option
    Args &describe(std::string option_name, std::string desc);

    //! Describe a long & short option
    Args &describe(std::string option_name, char short_name, std::string desc);

    //! Describe a long & short option with an arg
    Args &describe(std::string option_name, char short_name, std::string arg,
                   std::string desc);

    //! Set an option's default value
    Args &option_default(std::string name, std::string default_value);

    //! Return a help string containing described options
    std::string help(void) const;

  private:
    void reset_getopt(void);

    Args &describe_formatted(std::string option, std::string desc);
};

}; // namespace tdm

#endif /* UTIL_ARGPARSE_H */
