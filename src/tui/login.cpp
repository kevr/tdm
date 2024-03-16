// SPDX-License-Identifier: MIT
#include "login.h"
#include "color.h"
#include "lib/curses.h"
#include <functional>

using namespace std::placeholders;

namespace tdm::tui {

int Login::init(void)
{
    using lib::curses;

    // Initialize and clear the screen
    if (int e = m_screen.init(); e != 0)
        return e;
    curses->curs_set(0);
    curses->clear();
    curses->refresh();

    if (int e = resize(); e != 0) {
        return e;
    }

    m_box.on_draw([](auto &w) -> int {
        if (int e = w.box(); e != OK) {
            return ERR;
        }
        w.color_enable(ColorPair::GREEN);
        lib::curses->wmove(w.handle(), 0, 2);
        w.addstring(" Hello, color! ");
        w.color_disable(ColorPair::GREEN);
        w.refresh();
        return OK;
    });

    auto draw_box = [](auto &w) {
        w.background(ColorPair::IDLE_HIGHLIGHT);
        w.refresh();
        return 0;
    };
    m_power.on_draw(draw_box);
    m_desktop.on_draw(draw_box);

    return draw();
}

int Login::draw(void)
{
    m_screen.erase();
    try {
        call(std::bind(&Window::draw, &m_box));
        call(std::bind(&Window::draw, &m_power));
        call(std::bind(&Window::draw, &m_desktop));
    } catch (lib::CursesError &exc) {
        logger.error("{}", exc.what());
        return exc.error_code();
    }
    return OK;
}

int Login::resize(void)
{
    const auto [screen_w, screen_h] = m_screen.size();
    logger.debug("size(screen): {}x{}", screen_w, screen_h);

    const int box_w = 32, box_h = 8;
    const int box_x = (screen_w / 2) - (box_w / 2);
    const int box_y = (screen_h / 2) - (box_h / 2);
    logger.debug("size(center box): {}x{}+{}+{}", box_w, box_h, box_x, box_y);

    const int widget_size_w = 20;
    const int widget_size_h = 3;

    auto make_fn = [](Window &w) {
        return std::bind(&Window::init<ScreenDeleter>, &w, _1, _2, _3, _4, _5);
    };
    try {
        call(make_fn(m_box), m_screen, box_w, box_h, box_x, box_y);
        call(make_fn(m_power), m_screen, widget_size_w, widget_size_h,
             screen_w - widget_size_w, 0);
        call(make_fn(m_desktop), m_screen, widget_size_w, widget_size_h,
             screen_w - widget_size_w, screen_h - widget_size_h);
    } catch (lib::CursesError &exc) {
        logger.error("{}", exc.what());
        return exc.error_code();
    }

    return OK;
}

} // namespace tdm::tui
