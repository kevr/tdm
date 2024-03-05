// SPDX-License-Identifier: MIT
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
    // Globals
    virtual WINDOW *initscr(void) const;
    virtual int cbreak(void) const;
    virtual int noecho(void) const;
    virtual int keypad(WINDOW *, bool) const;
    virtual int refresh(void) const;
    virtual int clear(void) const;
    virtual int endwin(void) const;
};

inline Singleton<Curses> curses;
} // namespace tdm

#endif /* LIB_CURSES_H */
