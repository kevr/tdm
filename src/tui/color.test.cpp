// SPDX-License-Identifier: MIT
#include "color.h"
#include "../lib/mocks/curses.h"
#include "../lib/mocks/term.h"
#include <gtest/gtest.h>
#include <memory>

using namespace tdm;
using namespace tdm::lib;
using testing::_;
using testing::NiceMock;
using testing::Return;

class ColorTest : public testing::Test
{
  protected:
    std::shared_ptr<MockTerm> m_term = std::make_shared<MockTerm>();
    std::shared_ptr<NiceMock<MockCurses>> m_curses =
        std::make_shared<NiceMock<MockCurses>>();

  public:
    void SetUp(void)
    {
        term.set(m_term);
        curses.set(m_curses);
    }

    void TearDown(void)
    {
        curses.reset();
        term.reset();
    }
};

TEST_F(ColorTest, start_color_error)
{
    EXPECT_CALL(*m_curses, has_colors()).WillOnce(Return(TRUE));
    EXPECT_CALL(*m_term, tigetnum(_)).WillOnce(Return(8));
    EXPECT_CALL(*m_curses, start_color()).WillOnce(Return(ERR));

    int colors;
    EXPECT_EQ(tui::init_color(colors), ERR);
}
