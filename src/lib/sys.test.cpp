// SPDX-License-Identifier: MIT
#include "sys.h"
#include <gtest/gtest.h>

using namespace tdm;
using namespace tdm::lib;

TEST(sys, seteuid)
{
    EXPECT_EQ(sys->seteuid(getuid()), 0);
}

TEST(sys, setegid)
{
    EXPECT_EQ(sys->setegid(getgid()), 0);
}
