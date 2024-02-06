#include "app.h"
#include "lib/curses.h"
#include "util/logger.h"
#include "util/str.h"
#include "util/termio.h"
#include <fstream>

using namespace tdm;

namespace tdm {

App::App(void) = default;

App::~App(void)
{
    if (m_init) {
        tdm::curses->endwin();
        m_init = false;
    }
}

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

    logger.debug("curses init = {}", (m_init = curses->initscr() != nullptr));
    logger.debug("curses clear = {}", curses->clear());
    logger.debug("curses refresh = {}", curses->refresh());

    return 0;
}

void App::collect_users(const std::filesystem::path &passwd_file)
{
    std::ifstream ifs(passwd_file);
    m_users = get_users(ifs);
}
// End of defs
} // namespace tdm
