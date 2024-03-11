// SPDX-License-Identifier: MIT
#include "login.h"
#include "../lib/mocks/curses.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <regex>

using namespace tdm;
using namespace tdm::lib;
using testing::_;
using testing::NiceMock;
using testing::Return;

class TuiLoginTest : public testing::Test
{
  protected:
    std::shared_ptr<NiceMock<MockCurses>> m_curses =
        std::make_shared<NiceMock<MockCurses>>();

    WINDOW root;
    WINDOW children[3];

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
        EXPECT_CALL(*m_curses, initscr()).WillOnce(Return(&root));
    }

    void mock_derwin(void)
    {
        EXPECT_CALL(*m_curses, derwin(_, _, _, _, _))
            .WillOnce(Return(&children[0]))
            .WillOnce(Return(&children[1]))
            .WillOnce(Return(&children[2]));
    }
};

TEST_F(TuiLoginTest, screen_init_error)
{
    tui::Login login;
    EXPECT_EQ(login.init(), ERR);
}

TEST_F(TuiLoginTest, window_init_error)
{
    mock_initscr();

    tui::Login login;
    EXPECT_EQ(login.init(), ERR);
}

TEST_F(TuiLoginTest, box_error)
{
    mock_initscr();
    mock_derwin();
    EXPECT_CALL(*m_curses, wborder(_, _, _, _, _, _, _, _, _))
        .WillOnce(Return(ERR));

    tui::Login login;
    EXPECT_EQ(login.init(), ERR);
}

TEST_F(TuiLoginTest, delwin_error)
{
    // During this test, delwin errors are gracefully continued upon
    // after error logging
    mock_initscr();
    mock_derwin();
    EXPECT_CALL(*m_curses, delwin(_)).WillRepeatedly(Return(ERR));

    testing::internal::CaptureStdout();
    {
        tui::Login login;
        EXPECT_EQ(login.init(), OK);
    }
    auto output = testing::internal::GetCapturedStdout();

    std::regex re(R"(delwin(.*) returned [\-0-9]+)");
    EXPECT_TRUE(std::regex_search(output, re));
}

TEST_F(TuiLoginTest, success)
{
    mock_initscr();
    mock_derwin();

    tui::Login login;
    EXPECT_EQ(login.init(), OK);
}
