// SPDX-License-Identifier: MIT
#include "window.h"
#include <fmt/format.h>

namespace tdm::tui {

void WindowDeleter::operator()(WINDOW *win)
{
    if (int e = lib::curses->delwin(win); e != OK) {
        logger.error("delwin(0x{:2x}) returned {}",
                     reinterpret_cast<unsigned long>(win), e);
    }
}

} // namespace tdm::tui
