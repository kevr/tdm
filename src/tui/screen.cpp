// SPDX-License-Identifier: MIT
#include "screen.h"
#include "../util/logger.h"
#include <fmt/format.h>

namespace tdm::tui {

void ScreenDeleter::operator()(WINDOW *win)
{
    if (int e = lib::curses->endwin(); e != OK) {
        logger.error("endwin() of 0x{:2x} returned {}",
                     reinterpret_cast<unsigned long>(win), e);
    }
}

Screen::Screen(bool init)
{
    if (init) {
        if (int e = this->init(); e != 0) {
            std::string message =
                fmt::format("Screen::init() returned error {}", e);
            throw std::runtime_error(message);
        }
    }
}

int Screen::init(void)
{
    using lib::curses;

    setlocale(LC_ALL, "");
    auto win = curses->initscr();
    if (!win)
        return ERR;

    m_window.reset(win);
    curses->cbreak();
    curses->noecho();
    curses->keypad(BasicWindow::handle(), TRUE);

    return OK;
}

std::tuple<int, int> Screen::size(void) const
{
    return std::make_tuple(COLS, LINES);
}

} // namespace tdm::tui
