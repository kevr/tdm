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
