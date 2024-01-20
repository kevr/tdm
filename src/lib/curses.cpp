#include "curses.h"
using namespace tdm;

WINDOW *Curses::initscr(void) const { return ::initscr(); }
int Curses::endwin(void) const { return ::endwin(); }
