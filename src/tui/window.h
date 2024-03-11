// SPDX-License-Identifier: MIT
#ifndef TUI_WINDOW_H
#define TUI_WINDOW_H

#include "../lib/curses.h"
#include "../util/logger.h"
#include "basic_window.h"
#include <memory>

namespace tdm::tui {

struct WindowDeleter
{
    Logger logger{"Curses"};
    void operator()(WINDOW *);
};

class Window : public BasicWindow<WindowDeleter>
{
  public:
    Window(void) = default;

    template <typename Deleter>
    Window(BasicWindow<Deleter> &parent, int width, int height, int x, int y)
    {
        init(parent, width, height, x, y);
    }

    template <typename Deleter>
    int init(BasicWindow<Deleter> &parent, int width, int height, int x, int y)
    {
        WINDOW *win = lib::curses->derwin(parent.handle(), height, width, y, x);
        m_window.reset(win);
        return win != nullptr ? 0 : -1;
    }
};

} // namespace tdm::tui

#endif /* TUI_WINDOW_H */
