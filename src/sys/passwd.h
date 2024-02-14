// SPDX-License-Identifier: MIT
#ifndef SYS_PASSWD_H
#define SYS_PASSWD_H

#include "../freedesktop/desktop.h"
#include "../util/str.h"
#include <pwd.h>
#include <string>
#include <unistd.h>
#include <vector>

namespace tdm {

class User
{
  private:
    std::string m_name;
    uid_t m_uid;
    gid_t m_gid;
    std::string m_home;
    std::string m_shell;

  public:
    User(uid_t uid);
    User(const User &o);
    User(User &&o);

  public:
    User &operator=(const User &o);
    User &operator=(User &&o);

  public:
    const std::string &name(void) const;
    uid_t uid(void) const;
    gid_t gid(void) const;
    const std::string &home(void) const;
    const std::string &shell(void) const;

    /**
     * Populate instance with passwd information
     *
     * @throws std::invalid_argument When a user could not be found by uid
     **/
    User &populate(void);

    std::vector<freedesktop::DesktopFile> desktop_files(void);
};

std::vector<User> get_users(std::istream &passwd);

template <>
struct Join<User>
{
    static std::string call(const std::vector<User> &vec,
                            const std::string &sep)
    {
        std::vector<std::string> usernames;
        for (auto &u : vec)
            usernames.emplace_back(u.name());
        return Join<std::string>::call(usernames, sep);
    }
};
} // namespace tdm

#endif /* SYS_PASSWD_H */
