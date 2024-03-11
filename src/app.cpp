// SPDX-License-Identifier: MIT
#include "app.h"
#include "lib/curses.h"
#include "util/logger.h"
#include "util/str.h"
#include <fstream>
#include <set>

using namespace tdm;

namespace tdm {

int App::run(const std::filesystem::path &passwd_file)
{
    collect_users(passwd_file);
    logger.info("Users: {}", join(m_users, ", "));

    for (auto &user : m_users) {
        logger.info("{} desktops:", user.name());
        for (auto &desktop : user.desktop_files()) {
            logger.info("  - {}", desktop.path().string());
        }
    }

    if (int e = m_login.init(); e != 0) {
        logger.error("{} returned {}", typeid(m_login).name(), e);
        return e;
    }

    return 0;
}

void App::collect_users(const std::filesystem::path &passwd_file)
{
    std::ifstream ifs(passwd_file);
    m_users = get_users(ifs);
}

} // namespace tdm
