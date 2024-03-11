// SPDX-License-Identifier: MIT
#include "filesystem.h"
#include "../testing.h"
#include <fstream>
#include <gtest/gtest.h>

using namespace tdm;

class FilesystemTest : public testing::Test
{
  protected:
    tdm::test::TemporaryDirectory tmpdir;
};

TEST_F(FilesystemTest, listdir)
{
    std::ofstream(tmpdir.path() / "test") << "";
    std::ofstream(tmpdir.path() / "test.suffix") << "";

    // With no suffix provided
    auto files = tdm::listdir(tmpdir.c_str());
    EXPECT_EQ(files.size(), 2);

    // With a specified suffix
    files = tdm::listdir(tmpdir.c_str(), ".suffix");
    EXPECT_EQ(files.size(), 1);
}

TEST_F(FilesystemTest, listdir_empty)
{
    EXPECT_EQ(tdm::listdir(tmpdir.c_str()).size(), 0);
}
