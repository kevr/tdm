#ifndef STUBS_CURSES_H
#define STUBS_CURSES_H

#define F_OK 0

struct WINDOW
{};

WINDOW *initscr(void);
int endwin(void);

#endif /* STUBS_CURSES_H */
