// SPDX-License-Identifier: MIT
#ifndef TUI_BASIC_WINDOW_H
#define TUI_BASIC_WINDOW_H

#include "../lib/curses.h"
#include <functional>
#include <memory>

namespace tdm::tui {

template <typename Deleter>
class BasicWindow
{
  protected:
    std::unique_ptr<WINDOW, Deleter> m_window;
    std::function<int(BasicWindow &)> m_draw = [](BasicWindow &) {
        // Default draw function; do nothing
        return OK;
    };

  protected:
    BasicWindow(void) = default;

    BasicWindow(WINDOW *win) : m_window(win, Deleter{})
    {
    }

  public:
    BasicWindow &on_draw(std::function<int(BasicWindow &)> fn)
    {
        m_draw = fn;
        return *this;
    }

    int draw(void)
    {
        return m_draw(*this);
    }

    WINDOW *handle(void) const
    {
        return m_window.get();
    }

    int erase(void)
    {
        return lib::curses->werase(m_window.get());
    }

    int addstring(const char *str)
    {
        return lib::curses->waddstring(m_window.get(), str);
    }

    int box(void)
    {
        return lib::curses->wbox(m_window.get());
    }

    int background(short color_pair)
    {
        return lib::curses->wbkgd(m_window.get(), COLOR_PAIR(color_pair));
    }

    int color_enable(short color_pair)
    {
        return lib::curses->color_enable(m_window.get(), color_pair);
    }

    int color_disable(short color_pair)
    {
        return lib::curses->color_disable(m_window.get(), color_pair);
    }

    int refresh(void)
    {
        return lib::curses->wrefresh(m_window.get());
    }
};

} // namespace tdm::tui

#endif /* TUI_BASIC_WINDOW_H */
