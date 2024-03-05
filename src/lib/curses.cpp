// SPDX-License-Identifier: MIT
#include "curses.h"
using namespace tdm;

WINDOW *Curses::initscr(void) const
{
    return ::initscr();
}

int Curses::cbreak(void) const
{
    return ::cbreak();
}

int Curses::noecho(void) const
{
    return ::noecho();
}

int Curses::keypad(WINDOW *win, bool enabled) const
{
    return ::keypad(win, enabled);
}

int Curses::refresh(void) const
{
    return ::refresh();
}

int Curses::clear(void) const
{
    return ::clear();
}

int Curses::endwin(void) const
{
    return ::endwin();
}

WINDOW *Curses::derwin(WINDOW *orig, int nlines, int ncols, int begin_y,
                       int begin_x) const
{
    return ::derwin(orig, nlines, ncols, begin_y, begin_x);
}

int Curses::wclear(WINDOW *win) const
{
    return ::wclear(win);
}

int Curses::werase(WINDOW *win) const
{
    return ::werase(win);
}

int Curses::wrefresh(WINDOW *win) const
{
    return ::wrefresh(win);
}

int Curses::delwin(WINDOW *win) const
{
    return ::delwin(win);
}
