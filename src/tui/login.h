// SPDX-License-Identifier: MIT
#ifndef TUI_LOGIN_H
#define TUI_LOGIN_H

#include "screen.h"
#include "window.h"

namespace tdm::tui {

class Login
{
  private:
    Screen m_screen;

    Window m_box;
    Window m_user_selection;
    Window m_password;

    Window m_power;
    Window m_desktop;

  public:
    int draw(void);
    int resize(void);
    int init(void);

  private:
    template <typename F, typename... Args>
    void call(F func, Args &&...args)
    {
        if (int e = func(std::forward<Args>(args)...); e != OK) {
            throw lib::CursesError(e);
        }
    }
};

} // namespace tdm::tui

#endif /* TUI_LOGIN_H */
