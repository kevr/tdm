#include "exec.h"
#include "../lib/mocks/sys.h"
#include <errno.h>
#include <functional>
#include <gtest/gtest.h>
#include <memory>

using namespace tdm;
using namespace std::placeholders;
using testing::_;
using testing::Invoke;
using testing::Return;

class ExecTest : public testing::Test
{
  protected:
    Sys m_real_sys;
    std::shared_ptr<MockSys> m_sys = std::make_shared<MockSys>();

  public:
    void SetUp(void) { sys.set(m_sys); }
    void TearDown(void) { sys.reset(); }
};

TEST_F(ExecTest, getline_null_fd)
{
    std::string line;
    EXPECT_EQ(getline(nullptr, line), -1);
}

TEST_F(ExecTest, fcntl_exception)
{
    EXPECT_CALL(*m_sys, pipe(_))
        .WillOnce(Invoke(std::bind(&Sys::pipe, m_real_sys, _1)));
    EXPECT_CALL(*m_sys, fdopen(_, _))
        .WillOnce(Invoke(std::bind(&Sys::fdopen, m_real_sys, _1, _2)));
    EXPECT_CALL(*m_sys, popen(_, _))
        .WillOnce(Invoke(std::bind(&Sys::popen, m_real_sys, _1, _2)));
    EXPECT_CALL(*m_sys, fcntl(_, _, _))
        .WillOnce(Return(0))
        .WillOnce(Return(-1));

    EXPECT_THROW(Exec("echo")("test"), std::runtime_error);
}

TEST_F(ExecTest, pipe_fails)
{
    EXPECT_CALL(*m_sys, pipe(_)).WillOnce(Invoke([](int *) -> int {
        errno = EINVAL;
        return -1;
    }));
    Exec echo("echo");
    auto rc = echo("test");
    EXPECT_EQ(rc, EINVAL);
    EXPECT_EQ(echo.error(), strerror(EINVAL));
}

TEST_F(ExecTest, fdopen_fails)
{
    EXPECT_CALL(*m_sys, pipe(_))
        .WillOnce(Invoke(std::bind(&Sys::pipe, m_real_sys, _1)));
    EXPECT_CALL(*m_sys, fdopen(_, _))
        .WillOnce(Invoke([](int, const char *) -> FILE * {
            errno = EINVAL;
            return nullptr;
        }));
    Exec echo("echo");
    auto rc = echo("test");
    EXPECT_EQ(rc, EINVAL);
    EXPECT_EQ(echo.error(), strerror(EINVAL));
}

TEST_F(ExecTest, popen_fails)
{
    EXPECT_CALL(*m_sys, pipe(_))
        .WillOnce(Invoke(std::bind(&Sys::pipe, m_real_sys, _1)));
    EXPECT_CALL(*m_sys, fdopen(_, _))
        .WillOnce(Invoke(std::bind(&Sys::fdopen, m_real_sys, _1, _2)));
    EXPECT_CALL(*m_sys, popen(_, _))
        .WillOnce(Invoke([](const char *, const char *) -> FILE * {
            errno = EINVAL;
            return nullptr;
        }));
    Exec echo("echo");
    auto rc = echo("test");
    EXPECT_EQ(rc, EINVAL);
    EXPECT_EQ(echo.error(), strerror(EINVAL));
}

TEST(exec, stdout_works)
{
    Exec echo("echo");
    echo("test");

    std::string str;
    auto stdout_fn = [&str](std::string line) { str = line; };
    EXPECT_EQ(echo.communicate(stdout_fn, stdout_fn), 0);
    EXPECT_EQ(str, "test\n");
}

TEST(exec, stderr_works)
{
    Exec fake("FAKE_PROGRAM");
    fake("test");

    std::string str;
    auto stderr_fn = [&str](std::string line) { str = line; };
    EXPECT_EQ(fake.communicate(stderr_fn, stderr_fn), 127);
    EXPECT_EQ(str, "sh: line 1: FAKE_PROGRAM: command not found\n");
}