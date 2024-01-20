#include "argparse.h"
#include "termio.h"
#include <set>
using namespace tdm;

Args::Args(struct option *options) : m_options(options) {}

int Args::parse(int argc, char **argv)
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
    auto get_option = [&] {
        return getopt_long(argc, argv, getopt_fmt.c_str(), m_options, &idx);
    };

    int c = 0;
    while ((c = get_option()) != -1) {
        if (c == 0) {
            continue;
        } else if (c == '?') {
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

void Args::reset_getopt(void) { optind = 0; }
