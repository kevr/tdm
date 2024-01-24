#include "passwd.h"
#include "../lib/sys.h"
#include "../util/str.h"
#include <cstdint>
#include <format>
#include <fstream>

using namespace std::string_literals;

namespace tdm {

User::User(uid_t uid) : m_uid(uid) {}

User::User(const User &o)
{
    operator=(o);
}

User::User(User &&o)
{
    operator=(std::move(o));
}

User &User::operator=(const User &o)
{
    m_name = o.m_name;
    m_uid = o.m_uid;
    m_gid = o.m_gid;
    m_home = o.m_home;
    m_shell = o.m_shell;
    return *this;
}

User &User::operator=(User &&o)
{
    m_name = std::move(o.m_name);
    m_uid = o.m_uid;
    m_gid = o.m_gid;
    m_home = std::move(o.m_home);
    m_shell = std::move(o.m_shell);
    return *this;
}

const std::string &User::name(void) const { return m_name; }
uid_t User::uid(void) const { return m_uid; }
gid_t User::gid(void) const { return m_gid; }
const std::string &User::home(void) const { return m_home; }
const std::string &User::shell(void) const { return m_shell; }

User &User::populate(void)
{
    auto *passwd = sys->getpwuid(uid());
    if (!passwd) {
        throw std::invalid_argument(
            std::format("unable to lookup uid {}", uid()));
    }

    m_name = passwd->pw_name;
    m_gid = passwd->pw_gid;
    m_home = passwd->pw_dir;
    m_shell = passwd->pw_shell;
    return *this;
}

std::vector<User> get_users(std::istream &passwd)
{
    std::vector<User> users;

    std::string line;
    while (std::getline(passwd, line)) {
        line = strip(line);
        if (!line.size())
            continue;

        // Split line into (name, x, uid, ...)
        auto elements = split(line, ":", 3);
        uid_t uid = std::stoul(elements.at(2));

        // Only users with uids >= 1000 are collected.
        if (uid < 1000)
            continue;

        auto endswith = [](const std::string_view s,
                           const std::string_view suffix) -> bool {
            auto pos = s.size() - suffix.size();
            return s.find(suffix, pos) != std::string::npos;
        };

        auto user = User(uid).populate();
        const auto &sh = user.shell();
        if (!(endswith(sh, "nologin") || endswith(sh, "false"))) {
            users.emplace_back(std::move(user));
        }
    }

    return users;
}
};
