// SPDX-License-Identifier: MIT
#include "termio.h"
#include <gtest/gtest.h>
using testing::internal::CaptureStderr;
using testing::internal::CaptureStdout;
using testing::internal::GetCapturedStderr;
using testing::internal::GetCapturedStdout;

TEST(print, works)
{
    CaptureStdout();
    tdm::print("{}\n", "test");
    auto output = GetCapturedStdout();
    EXPECT_EQ(output, "test\n");
}

TEST(error, works)
{
    CaptureStderr();
    tdm::error(1, "{}\n", "test");
    auto output = GetCapturedStderr();
    EXPECT_EQ(output, "error: test\n");
}

TEST(error, returns_rc)
{
    int rc = tdm::error(0, "test\n");
    EXPECT_EQ(rc, 0);

    rc = tdm::error(1, "test\n");
    EXPECT_EQ(rc, 1);
}
