// SPDX-License-Identifier: MIT
#include "curses.h"

namespace tdm::lib {

WINDOW *Curses::initscr(void)
{
    return ::initscr();
}

int Curses::cbreak(void)
{
    return ::cbreak();
}

int Curses::noecho(void)
{
    return ::noecho();
}

int Curses::keypad(WINDOW *win, bool enabled)
{
    return ::keypad(win, enabled);
}

int Curses::refresh(void)
{
    return ::refresh();
}

int Curses::clear(void)
{
    return ::clear();
}

int Curses::endwin(void)
{
    return ::endwin();
}

WINDOW *Curses::derwin(WINDOW *orig, int nlines, int ncols, int begin_y,
                       int begin_x)
{
    return ::derwin(orig, nlines, ncols, begin_y, begin_x);
}

int Curses::wclear(WINDOW *win)
{
    return ::wclear(win);
}

int Curses::werase(WINDOW *win)
{
    return ::werase(win);
}

int Curses::wrefresh(WINDOW *win)
{
    return ::wrefresh(win);
}

int Curses::delwin(WINDOW *win)
{
    return ::delwin(win);
}

} // namespace tdm::lib
