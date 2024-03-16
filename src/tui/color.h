// SPDX-License-Identifier: MIT
#ifndef TUI_COLOR_H
#define TUI_COLOR_H

#include <map>
#include <ncurses.h>
#include <vector>

namespace tdm::tui {

enum ColorPair : short
{
    PRIMARY = 1,
    HIGHLIGHT = 2,
    IDLE_HIGHLIGHT = 3,
    GREEN = 4,
};

// No color, falls back to original color
const short COLOR_NONE = -1;

// Pairs of colors from 0-7 (8-color)
const std::vector<std::pair<short, short>> COLOR_8{
    {COLOR_WHITE, COLOR_NONE},
    {COLOR_BLACK, COLOR_WHITE},
    {COLOR_WHITE, COLOR_BLACK},
    {COLOR_GREEN, COLOR_NONE},
};

// Pairs of colors from 0-255 (256-color)
const std::vector<std::pair<short, short>> COLOR_256{
    {255, COLOR_NONE},
    {0, 255},
    {255, 242},
    {83, COLOR_NONE},
};

const std::map<int, std::vector<std::pair<short, short>>> COLOR_MAP{
    {8, COLOR_8},     // 8-color terminals: 8-colors
    {88, COLOR_8},    // 88-color terminals: 8-colors
    {256, COLOR_256}, // 256-color terminals: 256-colors
};

/**
 * Initialize ncurses colors based on COLOR_MAP
 *
 * Internally, calls start_color(), use_default_colors(), and init_pair()
 * for each pair defined in COLOR_MAP[num_colors].
 *
 * Two color schemas are supported: COLOR_8 and COLOR_256, which contain
 * n pairs where n = number of ColorPair enums.
 *
 * @param num_colors Assigned number of colors the terminal is capable of
 * @returns ERR if an internal ncurses call fails, OK otherwise
 **/
int init_color(int &num_colors);

} // namespace tdm::tui

#endif /* TUI_COLOR_H */
