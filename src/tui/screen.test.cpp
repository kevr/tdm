// SPDX-License-Identifier: MIT
#include "screen.h"
#include "../lib/mocks/curses.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

using namespace tdm;
using namespace tdm::lib;
using testing::NiceMock;
using testing::Return;

class ScreenTest : public testing::Test
{
  protected:
    std::shared_ptr<NiceMock<MockCurses>> m_curses =
        std::make_shared<NiceMock<MockCurses>>();

    WINDOW root;

  public:
    void SetUp(void)
    {
        curses.set(m_curses);
    }

    void TearDown(void)
    {
        curses.reset();
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
