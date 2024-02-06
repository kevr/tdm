#include "desktop.h"
#include "../except.h"
#include "../util/str.h"
#include "../util/termio.h"
#include <filesystem>
#include <fmt/format.h>
#include <fstream>
#include <set>
#include <sstream>

static std::set<char> skip_chars{
    '#',
    '[',
};

namespace tdm::freedesktop {

DesktopFile::DesktopFile(const std::filesystem::path &path)
{
    if (!std::filesystem::exists(path)) {
        auto str =
            fmt::format("unable to find {}, does it exist?", path.c_str());
        throw std::invalid_argument(str);
    }

    std::ifstream ifs(path.c_str());
    if (!ifs.is_open()) {
        auto str = fmt::format("unable to open {} for reading", path.c_str());
        throw std::runtime_error(str);
    }

    std::string buffer(std::istreambuf_iterator<char>(ifs), {});

    if (std::size_t n = parse(path, buffer); n != 0) {
        auto str =
            fmt::format("malformed line {} found in {}", n, path.c_str());
        throw tdm::parse_error(str);
    }
}

DesktopFile::DesktopFile(const DesktopFile &o)
    : m_path(o.m_path), m_options(o.m_options)
{
}

DesktopFile::DesktopFile(DesktopFile &&o)
    : m_path(std::move(o.m_path)), m_options(std::move(o.m_options))
{
}

DesktopFile &DesktopFile::operator=(const DesktopFile &o)
{
    m_path = o.m_path;
    m_options = o.m_options;
    return *this;
}

DesktopFile &DesktopFile::operator=(DesktopFile &&o)
{
    m_path = std::move(o.m_path);
    m_options = std::move(o.m_options);
    return *this;
}

const std::filesystem::path &DesktopFile::path(void) const { return m_path; }

std::size_t DesktopFile::parse(const std::filesystem::path &path,
                               std::string_view content)
{
    m_path = path;

    // Clear options if parse was previously run
    m_options.clear();

    // Parse a .desktop file
    std::istringstream ss(content.data());
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

const std::string &DesktopFile::get(const std::string &key) const
{
    return m_options.at(key);
}

}; // namespace tdm::freedesktop
