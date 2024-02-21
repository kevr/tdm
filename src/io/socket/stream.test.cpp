// SPDX-License-Identifier: MIT
#include "stream.h"
#include <gtest/gtest.h>

using namespace tdm::io;

TEST(Stream, construction)
{
    int fd = 1;
    socket::Stream stream(fd);
    EXPECT_EQ(stream.fd(), fd);

    // Move construction
    socket::Stream stream2(std::move(stream));
    EXPECT_EQ(stream.fd(), -1);
    EXPECT_EQ(stream2.fd(), fd);

    // Move assignment
    stream = std::move(stream2);
    EXPECT_EQ(stream2.fd(), -1);
    EXPECT_EQ(stream.fd(), fd);
}

TEST(Stream, getline)
{
    int fds[2];
    pipe(fds);

    std::string msg("test1\ntest2\n");
    EXPECT_GT(write(fds[1], msg.data(), msg.size() + 1), 0);
    close(fds[1]);

    socket::Stream stream(fds[0]);
    EXPECT_EQ(stream.soup(), "test1\ntest2\n");
    close(fds[0]);
}

TEST(Stream, getline_multiread)
{
    int fds[2];
    pipe(fds);

    std::string msg(256, 'a');
    msg.push_back('\n');
    EXPECT_GT(write(fds[1], msg.data(), msg.size() + 1), 0);
    close(fds[1]);

    socket::Stream stream(fds[0]);
    EXPECT_EQ(stream.soup().size(), 257);
    close(fds[0]);
}
