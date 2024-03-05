// SPDX-License-Identifier: MIT
#include "curses.h"
#include <algorithm>

WINDOW *initscr(void)
{
    return stdscr;
}

int cbreak(void)
{
    return F_OK;
}

int noecho(void)
{
    return F_OK;
}

int keypad(WINDOW *, int)
{
    return F_OK;
}

int refresh(void)
{
    return F_OK;
}

int clear(void)
{
    return F_OK;
}

int endwin(void)
{
    return F_OK;
}

WINDOW *derwin(WINDOW *, int, int, int, int)
{
    child_windows.emplace_back();
    return &child_windows.back();
}

int wclear(WINDOW *)
{
    return F_OK;
}

int werase(WINDOW *)
{
    return F_OK;
}

int wrefresh(WINDOW *)
{
    return F_OK;
}

int delwin(WINDOW *win)
{
    auto it = std::find_if(child_windows.begin(), child_windows.end(),
                           [win](auto &w) {
                               return &w == win;
                           });
    bool found = it != child_windows.end();
    if (found) {
        child_windows.erase(it);
        return F_OK;
    }

    return -1;
}
