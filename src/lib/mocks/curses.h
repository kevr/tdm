// SPDX-License-Identifier: MIT
#ifndef LIB_MOCKS_CURSES_H
#define LIB_MOCKS_CURSES_H

#include "../curses.h"
#include <gmock/gmock.h>

namespace tdm::lib {

class MockCurses : public Curses
{
  public:
    MOCK_METHOD(WINDOW *, initscr, (), (override));
    MOCK_METHOD(int, cbreak, (), (override));
    MOCK_METHOD(int, clear, (), (override));
    MOCK_METHOD(int, curs_set, (int), (override));
    MOCK_METHOD(int, noecho, (), (override));
    MOCK_METHOD(int, keypad, (WINDOW *, bool), (override));
    MOCK_METHOD(int, refresh, (), (override));
    MOCK_METHOD(int, getchar, (), (override));
    MOCK_METHOD(int, endwin, (), (override));

    MOCK_METHOD(int, waddstring, (WINDOW *, const char *), (override));

    MOCK_METHOD(WINDOW *, derwin, (WINDOW *, int, int, int, int), (override));
    MOCK_METHOD(int, werase, (WINDOW *), (override));
    MOCK_METHOD(int, wrefresh, (WINDOW *), (override));
    MOCK_METHOD(int, wborder,
                (WINDOW *, chtype, chtype, chtype, chtype, chtype, chtype,
                 chtype, chtype),
                (override));
    MOCK_METHOD(int, delwin, (WINDOW *), (override));

    MOCK_METHOD(bool, has_colors, (), (override));
    MOCK_METHOD(int, start_color, (), (override));
};

} // namespace tdm::lib

#endif /* LIB_MOCKS_CURSES_H */
