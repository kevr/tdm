// SPDX-License-Identifier: MIT
#ifndef STUBS_CURSES_H
#define STUBS_CURSES_H

#include <vector>

#define F_OK 0

struct WINDOW
{};

#define TRUE 1
#define FALSE 0

inline WINDOW root_window;
inline WINDOW *stdscr = &root_window;
inline std::vector<WINDOW> child_windows;

WINDOW *initscr(void);
int cbreak(void);
int noecho(void);
int keypad(WINDOW *, int);
int refresh(void);
int clear(void);
int endwin(void);

WINDOW *derwin(WINDOW *orig, int nlines, int ncols, int begin_y, int begin_x);
int wclear(WINDOW *win);
int werase(WINDOW *win);
int wrefresh(WINDOW *win);
int delwin(WINDOW *win);

#endif /* STUBS_CURSES_H */
