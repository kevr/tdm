// SPDX-License-Identifier: MIT
#ifndef TUI_SCREEN_H
#define TUI_SCREEN_H

#include "basic_window.h"
#include <tuple>

namespace tdm::tui {

struct ScreenDeleter
{
    void operator()(WINDOW *);
};

class Screen : public BasicWindow<ScreenDeleter>
{
  public:
    Screen(bool init = false);
    int init(void);

    //! Get current (width, height) tuple of the Screen's size
    std::tuple<int, int> size(void) const;
};

}

#endif /* TUI_SCREEN_H */
