// SPDX-License-Identifier: MIT
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

TEST(string, split_without_delimiter)
{
    std::string str("Key=Value1=Value2");
    auto v = split(str, "-");
    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], "Key=Value1=Value2");
}

TEST(string, split)
{
    std::string str("Key=Value1=Value2");
    auto v = split(str, "=");
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], "Key");
    EXPECT_EQ(v[1], "Value1");
    EXPECT_EQ(v[2], "Value2");
}

TEST(string, split_max_splits)
{
    std::string str("Key=Value1=Value2");
    auto v = split(str, "=", 1);
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], "Key");
    EXPECT_EQ(v[1], "Value1=Value2");
}
