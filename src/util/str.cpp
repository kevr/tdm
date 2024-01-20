#include "str.h"
#include <set>
using namespace tdm;

static std::set<char> whitespace{' ', '\t', '\n'};

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

std::string &strip(std::string &str)
{
    lstrip(str);
    return rstrip(str);
}

}; // namespace tdm
