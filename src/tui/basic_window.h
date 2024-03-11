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

    int box(void)
    {
        return lib::curses->wbox(m_window.get());
    }

    int refresh(void)
    {
        return lib::curses->wrefresh(m_window.get());
    }
};

} // namespace tdm::tui

#endif /* TUI_BASIC_WINDOW_H */
