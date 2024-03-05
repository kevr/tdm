// SPDX-License-Identifier: MIT
#include "curses.h"

static WINDOW root;

WINDOW *initscr(void)
{
    return &root;
}

int cbreak(void)
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
