#include "app.h"
#include <curses.h>

int App::run(void)
{
    initscr();
    m_init = true;
    return 0;
}

App::~App(void)
{
    if (m_init) {
        endwin();
        m_init = false;
    }
}
