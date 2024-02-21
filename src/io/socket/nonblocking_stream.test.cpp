// SPDX-License-Identifier: MIT
#include "nonblocking_stream.h"
#include "../../lib/mocks/sys.h"
#include <gtest/gtest.h>

using namespace tdm;
using namespace std::placeholders;
using testing::_;
using testing::Invoke;
using testing::Return;

class NonBlockingStreamTest : public testing::Test
{
  protected:
    std::shared_ptr<MockSys> m_sys = std::make_shared<MockSys>();
    int fds[2];

  public:
    void SetUp(void)
    {
        sys.set(m_sys);
        pipe(fds);
    }

    void TearDown(void)
    {
        ::close(fds[1]), ::close(fds[0]);
        sys.reset();
    }
};

TEST_F(NonBlockingStreamTest, works)
{
    EXPECT_CALL(*m_sys, fcntl(_, _, _))
        .WillRepeatedly(Invoke(std::bind(&Sys::fcntl, *sys, _1, _2, _3)));
    EXPECT_CALL(*m_sys, read(_, _, _))
        .WillOnce(Invoke([](auto, auto, auto) -> int {
            errno = EAGAIN;
            return -1;
        }))
        .WillRepeatedly(Invoke(std::bind(&Sys::read, *sys, _1, _2, _3)));

    std::string msg("test\n");
    EXPECT_GT(write(fds[1], msg.data(), msg.size() + 1), 0);

    io::socket::NonBlockingStream stream(fds[0]);
    std::string line;
    stream.getline(line);
    EXPECT_EQ(line, "test");
}

TEST_F(NonBlockingStreamTest, timeout)
{
    EXPECT_CALL(*m_sys, fcntl(_, _, _))
        .WillRepeatedly(Invoke(std::bind(&Sys::fcntl, *sys, _1, _2, _3)));
    io::socket::NonBlockingStream stream(fds[0]);
    std::string line;
    EXPECT_THROW(stream.timeout(1000 * 100).getline(line), std::out_of_range);
}

TEST_F(NonBlockingStreamTest, fcntl_fails)
{
    EXPECT_CALL(*m_sys, fcntl(_, _, _)).WillOnce(Return(-1));
    EXPECT_THROW(io::socket::NonBlockingStream stream(0), std::runtime_error);
}
