// SPDX-License-Identifier: MIT
#ifndef LIB_CURSES_H
#define LIB_CURSES_H

#include "../singleton.h"
#include <curses.h>
#include <fmt/format.h>

namespace tdm::lib {

struct CursesError : public std::runtime_error
{
  private:
    int m_error;

  public:
    CursesError(int error_code);
    int error_code(void) const;
};

class Curses
{
  public:
    virtual ~Curses(void) = default;

    // Globals
    virtual WINDOW *initscr(void);
    virtual int cbreak(void);
    virtual int curs_set(int visibility);
    virtual int noecho(void);
    virtual int keypad(WINDOW *, bool);
    virtual int refresh(void);
    virtual int clear(void);
    virtual int getchar(void); // getch()
    virtual int endwin(void);

    // Windows
    virtual WINDOW *derwin(WINDOW *orig, int nlines, int ncols, int begin_y,
                           int begin_x);
    virtual int waddstring(WINDOW *win, const char *str);
    virtual int werase(WINDOW *win);
    virtual int wborder(WINDOW *win, chtype ls, chtype rs, chtype ts, chtype bs,
                        chtype tl, chtype tr, chtype bl, chtype br);
    virtual int wbox(WINDOW *win);
    virtual int wrefresh(WINDOW *win);
    virtual int delwin(WINDOW *win);

    // Colors
    virtual int start_color(void);
    virtual int use_default_colors(void);
    virtual int init_pair(short pair, short f, short b);
    virtual bool has_colors(void);
    virtual int attribute_on(WINDOW *win, chtype attr);
    virtual int attribute_off(WINDOW *win, chtype attr);
    virtual int color_enable(WINDOW *win, short pair);
    virtual int color_disable(WINDOW *win, short pair);
    virtual int wbkgd(WINDOW *win, chtype ch);
    virtual int wmove(WINDOW *win, int y, int x);
};

inline Singleton<Curses> curses;
} // namespace tdm::lib

#endif /* LIB_CURSES_H */
