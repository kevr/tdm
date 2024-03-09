// SPDX-License-Identifier: MIT
#include "process.h"
#include "../lib/mocks/sys.h"
#include <gtest/gtest.h>

using namespace tdm;
using namespace tdm::lib;
using namespace std::placeholders;
using testing::_;
using testing::Invoke;
using testing::Return;
using testing::internal::CaptureStdout;
using testing::internal::GetCapturedStdout;

class ProcessTest : public testing::Test
{
  protected:
    Sys *m_real_sys;
    std::shared_ptr<MockSys> m_sys = std::make_shared<MockSys>();
    int m_pipefds[2];
    FILE *m_pipe;

  public:
    void SetUp(void)
    {
        logger.verbose(true);
        m_real_sys = sys.ptr();
        sys.set(m_sys);
    }

    void TearDown(void)
    {
        sys.reset();
    }

  protected:
    void mock_pipe(void)
    {
        pipe(m_pipefds);
        EXPECT_CALL(*m_sys, pipe(_))
            .WillRepeatedly(Invoke([this](int *pipedes) -> int {
                pipedes[0] = m_pipefds[0];
                pipedes[1] = m_pipefds[1];
                return 0;
            }));
    }

    void mock_dup2(void)
    {
        EXPECT_CALL(*m_sys, dup2(_, _)).WillRepeatedly(Return(0));
    }

    void mock_fork(pid_t pid)
    {
        EXPECT_CALL(*m_sys, fork()).WillRepeatedly(Return(pid));
    }

    /**
     * Mock the waitpid() function
     *
     * @param status_ A value for *status
     **/
    void mock_waitpid(int status_)
    {
        EXPECT_CALL(*m_sys, waitpid(_, _, _))
            .WillRepeatedly(
                Invoke([status_](pid_t pid, int *status, int) -> int {
                    *status = status_;
                    return pid;
                }));
    }

    void mock_execve(int rc)
    {
        EXPECT_CALL(*m_sys, execve(_, _, _)).WillRepeatedly(Return(rc));
    }
};

TEST_F(ProcessTest, pipe_fails)
{
    EXPECT_CALL(*m_sys, pipe(_)).WillOnce(Return(-1));

    Process echo("echo");
    EXPECT_THROW(echo.arg("test").start(), std::runtime_error);
}

TEST_F(ProcessTest, fork_fails)
{
    mock_pipe();
    EXPECT_CALL(*m_sys, fork()).WillOnce(Return(-1));

    Process echo("echo");
    EXPECT_EQ(echo.arg("test").start(), -1);
}

TEST_F(ProcessTest, waitpid_fails)
{
    mock_pipe();
    mock_fork(1);
    EXPECT_CALL(*m_sys, waitpid(_, _, _)).WillOnce(Return(-1));

    Process echo("echo");
    echo.arg("test").start();
    EXPECT_THROW(echo.wait(), std::runtime_error);
}

TEST_F(ProcessTest, fork_client)
{
    mock_pipe();
    mock_fork(0);
    mock_dup2();
    mock_execve(0);

    Process echo("echo");
    EXPECT_EQ(echo.arg("test").start(), 0);
}

TEST_F(ProcessTest, execve_fails)
{
    mock_pipe();
    mock_fork(0);
    mock_dup2();
    mock_execve(-1);

    Process echo("echo");

    CaptureStdout();
    EXPECT_EQ(echo.arg("test").start(), 0);
    auto output = GetCapturedStdout();
    EXPECT_NE(output.find("unable to execve echo"), std::string::npos);
}

TEST(Process, default_constructs)
{
    Process proc;
}

TEST(Process, empty_binary_throws)
{
    Process proc;
    EXPECT_THROW(proc.start(), std::invalid_argument);
}

TEST(Process, echo)
{
    Process echo("echo");
    EXPECT_EQ(echo.binary(), "echo");

    echo.arg("test").start();

    // Can't start it again until we kill it
    EXPECT_THROW(echo.start(), std::logic_error);

    EXPECT_EQ(echo.wait().return_code(), 0);

    std::string line;
    echo.stdout().getline(line);
    EXPECT_EQ(line, "test");
}

TEST(Process, cat_error)
{
    Process cat("cat");
    EXPECT_EQ(cat.binary(), "cat");

    cat.arg("/non-existing-file").start();

    // Exercise the move constructor
    Process cat2(std::move(cat));
    EXPECT_EQ(cat.binary(), "");
    EXPECT_EQ(cat2.binary(), "cat");
    EXPECT_NE(cat2.wait().return_code(), 0);

    std::string line;
    cat2.stderr().getline(line);
    EXPECT_NE(line.rfind("No such file or directory"), std::string::npos);
}

TEST(Process, missing_executable)
{
    Process proc("non-existing-exec");
    EXPECT_THROW(proc.start(), std::domain_error);
}

TEST(Process, child_signal)
{
    Process sleep("sleep");
    sleep.arg("30s").start();
    sleep.kill(SIGTERM);
    EXPECT_EQ(sleep.wait().return_code(), SIGTERM);
}

TEST(Process, env)
{
    Process printenv("printenv");
    printenv.env("TEST_VAR=1").start();
    EXPECT_EQ(printenv.wait().return_code(), 0);

    auto doc = printenv.stdout().soup();
    EXPECT_NE(doc.find("TEST_VAR=1"), std::string::npos);
}
