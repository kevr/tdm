// SPDX-License-Identifier: MIT
#ifndef STUBS_CURSES_H
#define STUBS_CURSES_H

#define F_OK 0

struct WINDOW
{};

WINDOW *initscr(void);
int cbreak(void);
int noecho(void);
int refresh(void);
int clear(void);
int endwin(void);

#endif /* STUBS_CURSES_H */
