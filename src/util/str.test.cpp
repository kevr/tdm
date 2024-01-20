#include "str.h"
#include <gtest/gtest.h>

using namespace tdm;

TEST(string, strip)
{
    std::string str(" test ");
    EXPECT_EQ(strip(str), "test");
}

TEST(string, lstrip)
{
    std::string str(" test ");
    EXPECT_EQ(lstrip(str), "test ");
}

TEST(string, rstrip)
{
    std::string str(" test");
    EXPECT_EQ(rstrip(str), " test");
}
