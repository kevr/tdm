#include "curses.h"
using namespace tdm;

WINDOW *Curses::initscr(void) const { return ::initscr(); }
int Curses::refresh(void) const { return ::refresh(); }
int Curses::endwin(void) const { return ::endwin(); }
