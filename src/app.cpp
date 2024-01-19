#include "app.h"
#include "lib/curses.h"

int App::run(void)
{
    m_init = tdm::curses->initscr() != nullptr;
    return 0;
}

App::~App(void)
{
    if (m_init) {
        tdm::curses->endwin();
        m_init = false;
    }
}
