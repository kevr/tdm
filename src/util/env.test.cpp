// SPDX-License-Identifier: MIT
#include "env.h"
#include "../sys/passwd.h"
#include <fmt/format.h>
#include <gtest/gtest.h>

using namespace tdm;

class EnvTest : public testing::Test
{
  protected:
    uid_t uid = 0;
    User me = User(uid).populate();

  public:
    void TearDown(void)
    {
        unsetenv("XDG_DATA_HOME");
    }
};

TEST_F(EnvTest, xdg_data_home)
{
    std::string home = getenv("HOME");
    EXPECT_EQ(tdm::xdg_data_home(me), "/root/.local/share");
}

TEST_F(EnvTest, xdg_data_home_set)
{
    setenv("XDG_DATA_HOME", "/test", 1);
    EXPECT_EQ(tdm::xdg_data_home(me), "/test");
}
