// SPDX-License-Identifier: MIT
#include "argparse.h"
#include <gtest/gtest.h>
using tdm::Args;

TEST(argparse, unknown_option)
{
    int argc = 2;
    const char *argv[] = {"tdm", "--blah"};

    struct option opts[] = {
        {nullptr, 0, nullptr, 0},
    };
    auto args = Args(opts);

    EXPECT_EQ(args.parse(argc, argv), 1);
}

TEST(argparse, option_with_argument)
{
    int argc = 3;
    const char *argv[] = {"tdm", "--arg", "test"};

    struct option opts[] = {
        {"arg", required_argument, nullptr, 'a'},
        {nullptr, 0, nullptr, 0},
    };
    auto args = Args(opts);

    EXPECT_EQ(args.parse(argc, argv), 0);
    EXPECT_EQ(args.get("arg"), "test");
}

TEST(argparse, defaults)
{
    int argc = 1;
    const char *argv[] = {"tdm"};

    struct option opts[] = {
        {"arg", required_argument, nullptr, 'a'},
        {nullptr, 0, nullptr, 0},
    };
    auto args = Args(opts).option_default("arg", "test");

    EXPECT_EQ(args.parse(argc, argv), 0);
    EXPECT_EQ(args.get("arg"), "test");
}
