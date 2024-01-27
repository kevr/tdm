#include "argparse.h"
#include "termio.h"
#include <algorithm>
#include <fmt/format.h>
#include <sstream>
using namespace tdm;

Args::Args(struct option *options) : m_options(options) {}

int Args::parse(int argc, const char **argv)
{
    // Clear out m_values in case these args were previously parsed
    m_values.clear();
    reset_getopt();

    std::map<int, int> short_opts;
    std::string getopt_fmt;
    struct option *opt = m_options;
    for (int i = 0; opt->name != nullptr; ++i, ++opt) {

        // If the option provided a short version
        if (opt->val > 1) {

            // Add the short char to getopt's format string
            getopt_fmt.push_back(opt->val);

            // Map the short char to the option's index
            short_opts[opt->val] = i;

            // If the option takes an argument, append ':'
            // to getopt's format string
            if (opt->has_arg) {
                getopt_fmt.push_back(':');
            }
        }
    }

    int idx = 0;
    char **argv_ = const_cast<char **>(argv);
    auto get_option = [&] {
        return getopt_long(argc, argv_, getopt_fmt.c_str(), m_options, &idx);
    };

    int c = 0;
    while ((c = get_option()) != -1) {
        if (c == '?') {
            // If the argument given is an unknown option,
            // return an error to the caller.
            return 1;
        }

        // Use m_options idx by default
        int i = idx;

        // If c is a short opt, get its m_options idx from our mapping
        if (short_opts.find(c) != short_opts.end()) {
            i = short_opts[c];
        }

        // Populate m_values with the option name with an optional
        // value; an empty string if no value is supplied.
        const char *name = m_options[i].name;
        m_values[name] = optarg ? optarg : "";
    }

    if (optind < argc) {
        return tdm::error(1, "positional arguments are not supported\n");
    }

    return 0;
}

bool Args::has(const std::string &opt) const
{
    return m_values.find(opt) != m_values.end();
}

Args &Args::describe(std::string option_name, std::string desc)
{
    return describe_formatted(fmt::format("--{}", option_name),
                              std::move(desc));
}

Args &Args::describe(std::string option_name, char short_name, std::string desc)
{
    return describe_formatted(fmt::format("-{}, --{}", short_name, option_name),
                              std::move(desc));
}

Args &Args::describe(std::string option_name, char short_name, std::string arg,
                     std::string desc)
{
    return describe_formatted(
        fmt::format("-{}, --{} {}", short_name, option_name, arg),
        std::move(desc));
}

Args &Args::option_default(std::string name, std::string default_value)
{
    m_defaults[name] = default_value;
    return *this;
}

std::string Args::help(void) const
{
    // Try to find the description element with the longest key.
    // This will be needed to calculate the proper padding for
    // each option display.
    auto longest =
        std::max_element(m_desc.begin(), m_desc.end(), [](auto &a, auto &b) {
            return a.first.size() < b.first.size();
        });

    // Defaulted to 0, max_len is set to the length of the longest
    // element if found.
    std::size_t max_len = 0;
    if (longest != m_desc.end()) {
        max_len = longest->first.size();
    }

    std::stringstream ss;
    ss << usage << "\n\nProgram options:\n";

    const std::string indent(2, ' ');
    const char *sep = "\u2022"; // UTF-8 Bullet
    for (auto &kv : m_desc) {
        std::size_t padding = max_len - kv.first.size();
        ss << indent << kv.first << std::string(padding, ' ') << indent << sep
           << ' ' << kv.second << '\n';
    }

    return ss.str();
}

void Args::reset_getopt(void) { optind = 0; }

Args &Args::describe_formatted(std::string option, std::string desc)
{
    m_desc.emplace_back(std::move(option), std::move(desc));
    return *this;
}
