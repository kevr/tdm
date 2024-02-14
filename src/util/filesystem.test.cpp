// SPDX-License-Identifier: MIT
#include "filesystem.h"
#include <fstream>
#include <gtest/gtest.h>

class FilesystemTest : public testing::Test
{
  protected:
    std::string tmpdir = "/tmp/tdm-XXXXXX";

  public:
    void SetUp(void)
    {
        mkdtemp(tmpdir.data());
    }

    void TearDown(void)
    {
        std::filesystem::remove_all(tmpdir);
    }
};

TEST_F(FilesystemTest, listdir)
{
    std::ofstream(tmpdir + "/test") << "";
    std::ofstream(tmpdir + "/test.suffix") << "";

    // With no suffix provided
    auto files = tdm::listdir(tmpdir);
    EXPECT_EQ(files.size(), 2);

    // With a specified suffix
    files = tdm::listdir(tmpdir, ".suffix");
    EXPECT_EQ(files.size(), 1);
}

TEST_F(FilesystemTest, listdir_empty)
{
    EXPECT_EQ(tdm::listdir(tmpdir).size(), 0);
}
