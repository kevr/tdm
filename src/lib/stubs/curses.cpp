#include "curses.h"

static WINDOW root;

WINDOW *initscr(void)
{
    return &root;
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
