#ifndef LIB_CURSES_H
#define LIB_CURSES_H

#include "../singleton.h"

#ifndef TDM_TEST
#include <curses.h>
#else
#include "stubs/curses.h"
#endif

namespace tdm {

class Curses
{
  public:
    virtual ~Curses(void) = default;

  public:
    WINDOW *initscr(void) const;
    int refresh(void) const;
    int clear(void) const;
    int endwin(void) const;
};

inline Singleton<Curses> curses;
};

#endif /* LIB_CURSES_H */
