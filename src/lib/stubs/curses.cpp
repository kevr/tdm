#include "curses.h"

static WINDOW root;

WINDOW *initscr(void) { return &root; }
int endwin(void) { return F_OK; }
