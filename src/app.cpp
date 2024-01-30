#include "app.h"
#include "lib/curses.h"
#include "util/logger.h"
#include "util/str.h"
#include <fstream>

using namespace tdm;

namespace tdm {

App::App(void) {}

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

    m_init = tdm::curses->initscr() != nullptr;
    return 0;
}

void App::collect_users(const std::filesystem::path &passwd_file)
{
    std::ifstream ifs(passwd_file);
    m_users = get_users(ifs);
}
// End of defs
} // namespace tdm
