// SPDX-License-Identifier: MIT
#include "color.h"
#include "../lib/curses.h"
#include "../lib/term.h"
#include "../util/logger.h"

using tdm::lib::curses;

namespace tdm::tui {

int init_color(int &num_colors)
{
    num_colors = 0;

    if (curses->has_colors()) {
        num_colors = lib::term->tigetnum("colors");
        logger.info("supported colors: {}", num_colors);
    }

    if (num_colors > 0) {
        if (curses->start_color() == ERR) {
            logger.error("unable to initialize terminal colors");
            return ERR;
        }

        curses->use_default_colors(); // Use terminal fg/bg by default

        // Initialize color pairs with ncurses
        auto colors = COLOR_MAP.at(num_colors);
        int i = 0;
        for (auto &kv : colors) {
            curses->init_pair(++i, kv.first, kv.second);
        }
    }

    return OK;
}

} // namespace tdm::tui
