// SPDX-License-Identifier: MIT
#include "screen.h"
#include "../lib/mocks/curses.h"
#include "../lib/mocks/term.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

using namespace tdm;
using namespace tdm::lib;
using testing::_;
using testing::NiceMock;
using testing::Return;

class ScreenTest : public testing::Test
{
  protected:
    std::shared_ptr<NiceMock<MockTerm>> m_term =
        std::make_shared<NiceMock<MockTerm>>();
    std::shared_ptr<NiceMock<MockCurses>> m_curses =
        std::make_shared<NiceMock<MockCurses>>();

    WINDOW root;

  public:
    void SetUp(void)
    {
        lib::term.set(m_term);
        curses.set(m_curses);
    }

    void TearDown(void)
    {
        curses.reset();
        lib::term.reset();
    }

  protected:
    void mock_initscr(void)
    {
        EXPECT_CALL(*m_curses, initscr()).WillRepeatedly(Return(&root));
    }
};

TEST_F(ScreenTest, init_fails)
{
    EXPECT_THROW(tui::Screen(true), std::runtime_error);

    tui::Screen screen;
    EXPECT_EQ(screen.init(), ERR);
}

TEST_F(ScreenTest, init_succeeds)
{
    mock_initscr();
    EXPECT_NO_THROW(tui::Screen(true));

    tui::Screen screen;
    EXPECT_EQ(screen.init(), OK);
}

TEST_F(ScreenTest, init_color_error)
{
    mock_initscr();
    EXPECT_CALL(*m_curses, has_colors()).WillOnce(Return(TRUE));
    EXPECT_CALL(*m_term, tigetnum(_)).WillOnce(Return(8));
    EXPECT_CALL(*m_curses, start_color()).WillOnce(Return(ERR));

    tui::Screen screen;
    EXPECT_EQ(screen.init(), ERR);
}

TEST_F(ScreenTest, endwin_fails)
{
    mock_initscr();
    EXPECT_CALL(*m_curses, endwin()).WillOnce(Return(ERR));
    EXPECT_NO_THROW(tui::Screen(true));
}

TEST_F(ScreenTest, default_draw)
{
    // Cover's BasicWindow::draw with default m_draw
    tui::Screen screen;
    EXPECT_NO_THROW(screen.draw());
}
