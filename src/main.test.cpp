// SPDX-License-Identifier: MIT
#define main tdm_main
#include "main.cpp"
#undef main

#include "lib/mocks/sys.h"
#include <gtest/gtest.h>
#include <memory>

using namespace tdm::lib;
using testing::_;
using testing::NiceMock;
using testing::Return;
using testing::internal::CaptureStderr;
using testing::internal::CaptureStdout;
using testing::internal::GetCapturedStderr;
using testing::internal::GetCapturedStdout;

class MainTest : public testing::Test
{
  protected:
    std::shared_ptr<NiceMock<MockSys>> m_sys =
        std::make_shared<NiceMock<MockSys>>();

  public:
    void SetUp(void)
    {
        sys.set(m_sys);
    }

    void TearDown(void)
    {
        sys.reset();
    }
};

TEST_F(MainTest, makedirs_error)
{
    EXPECT_CALL(*m_sys, exists(_)).WillOnce(Return(false));
    EXPECT_CALL(*m_sys, mkdir(_, _)).WillOnce(Return(-1));
    int argc = 1;
    const char *argv[] = {"tdm"};
    EXPECT_EQ(tdm_main(argc, argv), -1);
}

TEST(main, runs)
{
    int argc = 1;
    const char *argv[] = {"tdm"};
    EXPECT_EQ(tdm_main(argc, argv), 0);
}

TEST(main, help)
{
    int argc = 2;
    const char *argv[] = {"tdm", "--help"};

    CaptureStdout();
    EXPECT_EQ(tdm_main(argc, argv), 0);
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
    EXPECT_EQ(tdm_main(argc, argv), 0);
    auto help_output = GetCapturedStdout();

    argv[1] = "-h";
    CaptureStdout();
    EXPECT_EQ(tdm_main(argc, argv), 0);
    auto h_output = GetCapturedStdout();

    EXPECT_EQ(help_output, h_output);
}

TEST(main, version)
{
    int argc = 2;
    const char *argv[] = {"tdm", "--version"};

    CaptureStdout();
    EXPECT_EQ(tdm_main(argc, argv), 0);
    auto output = GetCapturedStdout();
    EXPECT_EQ(output, fmt::format("{}\n", PROJECT_VER));
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
    const char *argv[] = {"tdm", "--log-to", "/sys/fake"};

    CaptureStderr();
    EXPECT_EQ(tdm_main(argc, argv), 1);
    auto output = GetCapturedStderr();
    EXPECT_EQ(output, "error: unable to open '/sys/fake' for writing\n");
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
