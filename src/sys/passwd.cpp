// SPDX-License-Identifier: MIT
#include "passwd.h"
#include "../lib/sys.h"
#include "../util/container.h"
#include "../util/env.h"
#include "../util/filesystem.h"
#include "../util/logger.h"
#include "../util/str.h"
#include <fmt/format.h>

using namespace std::string_literals;
using tdm::freedesktop::DesktopFile;

namespace tdm {

User::User(struct passwd *pwd)
    : m_name(pwd->pw_name), m_uid(pwd->pw_uid), m_gid(pwd->pw_gid),
      m_home(pwd->pw_dir), m_shell(pwd->pw_shell)
{
}

User::User(uid_t uid) : m_uid(uid)
{
}

User::User(const std::string &name) : m_name(name)
{
}

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

const std::string &User::name(void) const
{
    return m_name;
}

uid_t User::uid(void) const
{
    return m_uid.value();
}

gid_t User::gid(void) const
{
    return m_gid;
}

const std::string &User::home(void) const
{
    return m_home;
}

const std::string &User::shell(void) const
{
    return m_shell;
}

std::vector<DesktopFile> User::desktop_files(void)
{
    auto collect = [](std::vector<std::filesystem::path> listing,
                      std::map<std::string, DesktopFile> &results) {
        for (auto &path : listing) {
            try {
                freedesktop::DesktopFile desktop(path);
                const auto name = desktop.get("Name");
                results[name] = std::move(desktop);
            } catch (std::exception &exc) {
                logger.error("{}", exc.what());
                continue;
            }
        }
    };

    std::map<std::string, DesktopFile> results;

    // First, try collecting system xsessions
    collect(listdir("/usr/share/xsessions", ".desktop"), results);

    // After, try collecting XDG_DATA_HOME xsessions, overriding
    // system xsessions of the same name
    auto user_xsessions =
        std::filesystem::path(xdg_data_home(*this)) / "xsessions";
    collect(listdir(user_xsessions, ".desktop"), results);

    return values(results);
}

User &User::populate(void)
{
    struct passwd *pwd = m_uid.has_value() ? lib::sys->getpwuid(uid())
                                           : lib::sys->getpwnam(name().c_str());

    if (!pwd) {
        std::string msg("unable to lookup user with ");
        if (m_uid.has_value()) {
            msg.append(fmt::format("uid = {}", uid()));
        } else {
            msg.append(fmt::format("name = '{}'", name()));
        }

        throw std::invalid_argument(msg);
    }

    m_name = pwd->pw_name;
    m_uid = pwd->pw_uid;
    m_gid = pwd->pw_gid;
    m_home = pwd->pw_dir;
    m_shell = pwd->pw_shell;
    return *this;
}

std::vector<User> get_users(const std::filesystem::path &passwd_db)
{
    FILE *passwd = fopen(passwd_db.c_str(), "r");
    if (!passwd) {
        std::string message =
            fmt::format(R"(fopen("{}") failed)", passwd_db.c_str());
        throw std::runtime_error(message);
    }

    std::vector<User> users;
    struct passwd *pwd;
    while ((pwd = fgetpwent(passwd)) != nullptr) {
        if (pwd->pw_uid < 1000) {
            continue;
        }
        const char *sh = pwd->pw_shell;
        if (!(endswith(sh, "nologin") || endswith(sh, "false"))) {
            users.emplace_back(User(pwd));
        }
    }

    return users;
} // LCOV_EXCL_LINE

} // namespace tdm
