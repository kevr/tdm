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
    EXPECT_EQ(tdm_main(argc, argv), F_OK);
}

TEST(main, help)
{
    int argc = 2;
    const char *argv[] = {"tdm", "--help"};

    CaptureStdout();
    EXPECT_EQ(tdm_main(argc, argv), F_OK);
    auto output = GetCapturedStdout();
    EXPECT_NE(output.find(tdm::Args::usage), std::string::npos);
    EXPECT_NE(output.find("-h, --help"), std::string::npos);
    EXPECT_NE(output.find("--version"), std::string::npos);
}

TEST(main, help_short)
{
    // Test that output from `tdm -h` matches `tdm --help`
    int argc = 2;
    const char *argv[] = {"tdm", "--help"};

    CaptureStdout();
    EXPECT_EQ(tdm_main(argc, argv), F_OK);
    auto help_output = GetCapturedStdout();

    argv[1] = "-h";
    CaptureStdout();
    EXPECT_EQ(tdm_main(argc, argv), F_OK);
    auto h_output = GetCapturedStdout();

    EXPECT_EQ(help_output, h_output);
}

TEST(main, version)
{
    int argc = 2;
    const char *argv[] = {"tdm", "--version"};

    CaptureStdout();
    EXPECT_EQ(tdm_main(argc, argv), F_OK);
    auto output = GetCapturedStdout();
    EXPECT_EQ(output, std::format("{}\n", PROJECT_VER));
}

TEST(main, logs_to_logfile)
{
    int argc = 3;
    const char *argv[] = {"tdm", "--log-to", "/dev/stdout"};
    CaptureStdout();
    EXPECT_EQ(tdm_main(argc, argv), 0);
    auto output = GetCapturedStdout();
    EXPECT_NE(output.find("started logging to '/dev/stdout'\n"),
              std::string::npos);
}

TEST(main, unable_to_open_logfile)
{
    int argc = 3;
    const char *argv[] = {"tdm", "--log-to", "/dev/fake"};

    CaptureStderr();
    EXPECT_EQ(tdm_main(argc, argv), 1);
    auto output = GetCapturedStderr();
    EXPECT_EQ(output, "error: unable to open '/dev/fake' for writing\n");
}

TEST(main, gracefully_fails)
{
    int argc = 2;
    const char *argv[] = {"tdm", "test"};

    CaptureStderr();
    EXPECT_EQ(tdm_main(argc, argv), 1);
    auto output = GetCapturedStderr();
    EXPECT_EQ(output, "error: positional arguments are not supported\n");
}
