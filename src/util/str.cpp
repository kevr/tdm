#include "str.h"
#include <set>
using namespace tdm;

const std::set<char> whitespace{' ', '\t', '\r', '\n'};

namespace tdm {

std::string &strip_at(std::string &str, std::size_t pos, int inc)
{
    auto it = str.begin() + pos;
    while (whitespace.find(*it) != whitespace.end()) {
        str.erase(it);
        it += inc;
    }
    return str;
}

std::string &lstrip(std::string &str)
{
    // Strip whitespace starting from the beginning
    return strip_at(str, 0, 1);
}

std::string &rstrip(std::string &str)
{
    // Strip whitespace starting at the end
    return strip_at(str, str.size() - 1, -1);
}

std::string &strip(std::string &str) { return rstrip(lstrip(str)); }

std::vector<std::string> split(const std::string &str,
                               const std::string &delimiter,
                               std::size_t max_splits)
{
    std::vector<std::string> output;

    std::size_t i = 0, o = 0;
    i = str.find(delimiter, o);
    for (std::size_t splits = 0;
         (!max_splits || splits < max_splits) && i != std::string::npos;
         ++splits) {
        output.emplace_back(str.substr(o, i - o));
        o = i + 1;
        i = str.find(delimiter, o);
    }

    std::string last(str.substr(o, str.size() - o));
    if (last.size()) {
        output.emplace_back(str.substr(o, str.size() - o));
    }

    return output;
}

std::tuple<int, std::string> getline(FILE *file)
{
    char *line = nullptr;
    size_t buffer_size = 0;
    int bytes = ::getline(&line, &buffer_size, file);
    std::string line_;
    if (bytes > 0) {
        line_ = line;
        free(line);
    }
    return std::make_tuple(bytes, rstrip(line_));
}

}; // namespace tdm
