// SPDX-License-Identifier: MIT
#include "curses.h"

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
