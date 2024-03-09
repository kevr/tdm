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

    // Globals
    virtual WINDOW *initscr(void);
    virtual int cbreak(void);
    virtual int noecho(void);
    virtual int keypad(WINDOW *, bool);
    virtual int refresh(void);
    virtual int clear(void);
    virtual int endwin(void);

    // Windows
    virtual WINDOW *derwin(WINDOW *orig, int nlines, int ncols, int begin_y,
                           int begin_x);
    virtual int wclear(WINDOW *win);
    virtual int werase(WINDOW *win);
    virtual int wrefresh(WINDOW *win);
    virtual int delwin(WINDOW *win);
};

inline Singleton<Curses> curses;
} // namespace tdm

#endif /* LIB_CURSES_H */
