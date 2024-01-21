#include "desktop.h"
#include "../util/str.h"
#include "../util/termio.h"
#include <set>
#include <sstream>

static std::set<char> skip_chars{
    '#',
    '[',
};

namespace tdm::freedesktop {

DesktopFile::DesktopFile(const DesktopFile &o) : m_options(o.m_options) {}
DesktopFile::DesktopFile(DesktopFile &&o) : m_options(std::move(o.m_options)) {}

DesktopFile &DesktopFile::operator=(const DesktopFile &o)
{
    m_options = o.m_options;
    return *this;
}

DesktopFile &DesktopFile::operator=(DesktopFile &&o)
{
    m_options = std::move(o.m_options);
    return *this;
}

std::size_t DesktopFile::parse(std::string_view sv)
{
    // Clear options if parse was previously run
    m_options.clear();

    // Parse a .desktop file
    std::istringstream ss(sv.data());
    std::string line;
    for (std::size_t i = 1; std::getline(ss, line); ++i) {
        bool skip = strip(line).size() == 0 ||
                    skip_chars.find(line[0]) != skip_chars.end();
        if (skip)
            continue;

        auto kv = split(line, "=", 1);
        if (kv.size() != 2) {
            // A valid "Key=Value" option was unable to be parsed
            return i;
        }

        auto &key = strip(kv[0]);
        const auto &value = strip(kv[1]);

        // Force-capitalize the first char so get() users can expect it
        key[0] = toupper(key[0]);
        m_options[key] = value;
    }

    return 0;
}

std::string DesktopFile::get(const std::string &key) const
{
    return m_options.at(key);
}

}; // namespace tdm::freedesktop
