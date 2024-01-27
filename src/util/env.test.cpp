#include "env.h"
#include <fmt/format.h>
#include <gtest/gtest.h>

class EnvTest : public testing::Test
{
  public:
    void TearDown(void) { unsetenv("XDG_DATA_HOME"); }
};

TEST_F(EnvTest, xdg_data_home)
{
    std::string home = getenv("HOME");
    EXPECT_EQ(tdm::xdg_data_home(), fmt::format("{}/.local/share", home));
}

TEST_F(EnvTest, xdg_data_home_set)
{
    setenv("XDG_DATA_HOME", "/test", 1);
    EXPECT_EQ(tdm::xdg_data_home(), "/test");
}
