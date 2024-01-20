#define main tdm_main
#include "main.cpp"
#undef main

#include <gtest/gtest.h>

using testing::internal::CaptureStderr;
using testing::internal::CaptureStdout;
using testing::internal::GetCapturedStderr;
using testing::internal::GetCapturedStdout;

TEST(main, runs)
{
    int argc = 1;
    const char *argv[] = {"tdm"};
    EXPECT_EQ(tdm_main(argc, const_cast<char **>(argv)), F_OK);
}

TEST(main, version)
{
    int argc = 2;
    const char *argv[] = {"tdm", "--version"};

    CaptureStdout();
    EXPECT_EQ(tdm_main(argc, const_cast<char **>(argv)), F_OK);
    auto output = GetCapturedStdout();
    EXPECT_EQ(output, std::format("{}\n", PROJECT_VER));
}

TEST(main, gracefully_fails)
{
    int argc = 2;
    const char *argv[] = {"tdm", "test"};
    CaptureStderr();
    EXPECT_EQ(tdm_main(argc, const_cast<char **>(argv)), 1);
    auto output = GetCapturedStderr();
    EXPECT_EQ(output, "error: positional arguments are not supported\n");
}
