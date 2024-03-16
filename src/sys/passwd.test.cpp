// SPDX-License-Identifier: MIT
#include "passwd.h"
#include "../lib/mocks/sys.h"
#include "../testing.h"
#include "../util/termio.h"
#include "gtest/gtest.h"
#include <fstream>
#include <gtest/gtest.h>

using namespace tdm;
using namespace tdm::lib;
using testing::_;
using testing::Return;
using testing::internal::CaptureStdout;
using testing::internal::GetCapturedStdout;

class PasswdTest : public testing::Test
{
  protected:
    std::shared_ptr<MockSys> m_sys = std::make_shared<MockSys>();
    test::TemporaryDirectory tmpdir;
    std::filesystem::path passwd_db = tmpdir.path() / "passwd";

  public:
    void SetUp(void)
    {
        sys.set(m_sys);
    }

    void TearDown(void)
    {
        sys.reset();
    }

  protected:
    void write_passwd(const std::filesystem::path &passwd_db,
                      const std::string_view &content)
    {
        std::ofstream ofs(passwd_db, std::ios::out);
        ofs << content << "\n";
        ofs.close();
    }
};

TEST_F(PasswdTest, missing_passwd_file)
{
    EXPECT_THROW(tdm::get_users(tmpdir.path() / "passwd"), std::runtime_error);
}

TEST_F(PasswdTest, uid_fails)
{
    EXPECT_CALL(*m_sys, getpwuid(_)).WillOnce(Return(nullptr));
    uid_t uid = 0;
    EXPECT_THROW(User(uid).populate(), std::invalid_argument);
}

TEST_F(PasswdTest, missing_segments)
{
    // TODO: Do these checks properly within get_users and throw if validation
    // fails
    write_passwd(passwd_db, "username:x:1000:1000");
    auto users = tdm::get_users(tmpdir.path() / "passwd");
    EXPECT_EQ(users.size(), 1);
    EXPECT_EQ(users[0].name(), "username");
    EXPECT_EQ(users[0].uid(), 1000);
    EXPECT_EQ(users[0].gid(), 1000);
    EXPECT_EQ(users[0].home(), "");
    EXPECT_EQ(users[0].shell(), "");
}

TEST_F(PasswdTest, user)
{
    write_passwd(passwd_db, R"(
noop:x:999:999::/:/usr/bin/nologin
test:x:1000:1000::/home/test:/usr/bin/bash
test2:x:1001:1001::/home/test2:/usr/bin/bash
test3:x:1002:1002::/home/test3:/usr/bin/bash
)");

    auto users = tdm::get_users(tmpdir.path() / "passwd");
    EXPECT_EQ(users.size(), 3);

    auto &test = users.at(0);
    EXPECT_EQ(test.name(), "test");
    EXPECT_EQ(test.gid(), 1000);
    EXPECT_EQ(test.home(), "/home/test");
    EXPECT_EQ(test.shell(), "/usr/bin/bash");
}

TEST_F(PasswdTest, skips_invalid_desktop_file)
{
    m_sys.reset();

    auto user = User(getuid()).populate();
    size_t num_desktops = user.desktop_files().size();

    setenv("XDG_DATA_HOME", tmpdir.c_str(), 1);
    auto xsessions = std::filesystem::path(tmpdir) / "xsessions";
    mkdir(xsessions.c_str(), 0755);

    // Produce a valid .desktop file
    auto desktop = xsessions / "test.desktop";
    std::ofstream ofs(desktop, std::ios::out);
    ofs << "[Desktop entry]\n"
        << "Name = test\n"
        << "Exec = test\n";
    ofs.close();

    // Produce an invalid .desktop file
    desktop = xsessions / "invalid.desktop";
    ofs.open(desktop, std::ios::out);
    ofs << "[Desktop entry]\n"
        << "Name = test\n"
        << "Exec\n";
    ofs.close();

    CaptureStdout();
    size_t new_size = user.desktop_files().size();
    auto output = GetCapturedStdout();
    tdm::print("{}", output);

    // Expect that test.desktop was parsed and added to results,
    // but invalid.desktop was excluded
    EXPECT_EQ(new_size, num_desktops + 1);

    // Expect that we got a log error about a malformed line
    EXPECT_NE(output.find("malformed line"), std::string::npos);

    unsetenv("XDG_DATA_HOME");
}

TEST(passwd, root_user)
{
    EXPECT_NE(sys->getpwuid(0), nullptr);
}
