#define main tdm_main
#include "main.cpp"
#undef main

#include <gtest/gtest.h>

TEST(main, runs)
{
    int argc = 1;
    const char *argv[] = {"tdm"};
    EXPECT_EQ(tdm_main(argc, const_cast<char **>(argv)), F_OK);
}
