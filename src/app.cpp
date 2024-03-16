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

    static const std::set<int> error_chars{-1, 'q'};
    int ch;
    do {
        ch = lib::curses->getchar();

        switch (ch) {
        case KEY_RESIZE:
            m_login.resize();
            m_login.draw();
            break;
        }
    } while (error_chars.find(ch) == error_chars.end());

    if (ch == -1) {
        logger.error("getch() returned -1");
        return -1;
    } else {
        logger.debug("user pressed 'q', quitting...");
    }

    return 0;
}

void App::collect_users(const std::filesystem::path &passwd_file)
{
    m_users = get_users(passwd_file);
}

} // namespace tdm
