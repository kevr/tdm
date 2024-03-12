// SPDX-License-Identifier: MIT
#include "filesystem.h"
#include "../testing.h"
#include <fstream>
#include <gtest/gtest.h>
#include <sys/stat.h>

using namespace tdm;

class FilesystemTest : public testing::Test
{
  protected:
    test::TemporaryDirectory tmpdir;
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

TEST_F(FilesystemTest, makedirs)
{
    EXPECT_EQ(makedirs(tmpdir.path() / "test"), 0);
}

TEST_F(FilesystemTest, makedirs_failed)
{
    chmod(tmpdir.c_str(),
          S_IRUSR | S_IRGRP | S_IROTH | S_IXUSR | S_IXGRP | S_IXOTH);
    EXPECT_EQ(makedirs(tmpdir.path() / "test"), -1);
}

TEST_F(FilesystemTest, makedirs_permission_denied)
{
    chmod(tmpdir.c_str(), S_IRUSR | S_IRGRP | S_IROTH);
    EXPECT_EQ(makedirs(tmpdir.path() / "test"), -1);
}
