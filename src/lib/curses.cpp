// SPDX-License-Identifier: MIT
#include "curses.h"
#include "../util/logger.h"

namespace tdm::lib {

CursesError::CursesError(int error_code)
    : std::runtime_error(fmt::format("error_code = {}", error_code)),
      m_error(error_code)
{
}

int CursesError::error_code(void) const
{
    return m_error;
}

WINDOW *Curses::initscr(void)
{
    return ::initscr();
}

int Curses::cbreak(void)
{
    return ::cbreak();
}

int Curses::curs_set(int visibility)
{
    return ::curs_set(visibility);
}

int Curses::noecho(void)
{
    return ::noecho();
}

int Curses::keypad(WINDOW *win, bool enabled)
{
    return ::keypad(win, enabled);
}

int Curses::refresh(void)
{
    return ::refresh();
}

int Curses::clear(void)
{
    return ::clear();
}

int Curses::getchar(void)
{
#ifndef TDM_TEST
    // The following macro-conditional variable is excluded from
    // coverage, as ::getch() is used only in production and
    // can be depended on.
    int ch = ::getch(); // LCOV_EXCL_LINE
#else
    int ch = 'q';
#endif
    std::string output = ch >= 0 && ch <= 255
                             ? "'" + std::string(1, static_cast<char>(ch)) + "'"
                             : std::to_string(ch);
    logger.debug("keypress: {}", output);
    return ch;
}

int Curses::endwin(void)
{
    return ::endwin();
}

WINDOW *Curses::derwin(WINDOW *orig, int nlines, int ncols, int begin_y,
                       int begin_x)
{
    return ::derwin(orig, nlines, ncols, begin_y, begin_x);
}

int Curses::waddstring(WINDOW *win, const char *str)
{
    return ::waddstr(win, str);
}

int Curses::werase(WINDOW *win)
{
    return ::werase(win);
}

int Curses::wborder(WINDOW *win, chtype ls, chtype rs, chtype ts, chtype bs,
                    chtype tl, chtype tr, chtype bl, chtype br)
{
    return ::wborder(win, ls, rs, ts, bs, tl, tr, bl, br);
}

int Curses::wbox(WINDOW *win)
{
    return this->wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE,
                         ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER,
                         ACS_LRCORNER);
}

int Curses::wrefresh(WINDOW *win)
{
    return ::wrefresh(win);
}

int Curses::delwin(WINDOW *win)
{
    return ::delwin(win);
}

int Curses::start_color(void)
{
    return ::start_color();
}

int Curses::use_default_colors(void)
{
    return ::use_default_colors();
}

int Curses::init_pair(short pair, short f, short b)
{
    return ::init_pair(pair, f, b);
}

bool Curses::has_colors(void)
{
    return ::has_colors();
}

int Curses::attribute_on(WINDOW *win, chtype attr)
{
    return wattron(win, attr);
}

int Curses::attribute_off(WINDOW *win, chtype attr)
{
    return wattroff(win, attr);
}

int Curses::color_enable(WINDOW *win, short pair)
{
    return attribute_on(win, COLOR_PAIR(pair));
}

int Curses::color_disable(WINDOW *win, short pair)
{
    return attribute_off(win, COLOR_PAIR(pair));
}

int Curses::wbkgd(WINDOW *win, chtype ch)
{
    return ::wbkgd(win, ch);
}

int Curses::wmove(WINDOW *win, int y, int x)
{
    return ::wmove(win, y, x);
}

} // namespace tdm::lib
