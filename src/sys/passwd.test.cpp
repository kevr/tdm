// SPDX-License-Identifier: MIT
#include "passwd.h"
#include "../lib/mocks/sys.h"
#include "../util/termio.h"
#include "gtest/gtest.h"
#include <fstream>
#include <gtest/gtest.h>

using tdm::sys;
using tdm::User;
using testing::_;
using testing::Return;
using testing::internal::CaptureStdout;
using testing::internal::GetCapturedStdout;

class PasswdTest : public testing::Test
{
  protected:
    std::shared_ptr<tdm::MockSys> m_sys;
    std::string tmpdir = "/tmp/tdm-XXXXXX";

  public:
    void SetUp(void)
    {
        m_sys = std::make_shared<tdm::MockSys>();
        sys.set(m_sys);

        mkdtemp(tmpdir.data());
    }

    void TearDown(void)
    {
        std::filesystem::remove_all(tmpdir);
        sys.reset();
    }
};

TEST_F(PasswdTest, unknown_user)
{
    EXPECT_CALL(*m_sys, getpwuid(_)).WillOnce(Return(nullptr));
    std::stringstream ss;
    ss << "username:x:1000:1000\n";
    EXPECT_THROW(tdm::get_users(ss), std::invalid_argument);
}

struct passwd make_passwd(const char *name, gid_t gid, const char *home,
                          const char *shell)
{
    struct passwd pwd;
    memset(&pwd, 0, sizeof(pwd));
    pwd.pw_name = const_cast<char *>(name);
    pwd.pw_gid = gid;
    pwd.pw_dir = const_cast<char *>(home);
    pwd.pw_shell = const_cast<char *>(shell);
    return pwd;
}

TEST_F(PasswdTest, user)
{
    struct passwd pwd =
        make_passwd("test", 1000, "/home/test", "/usr/bin/bash");
    struct passwd pwd2 =
        make_passwd("test2", 1001, "/home/test2", "/usr/bin/nologin");
    struct passwd pwd3 =
        make_passwd("test3", 1002, "/home/test3", "/usr/bin/false");

    EXPECT_CALL(*m_sys, getpwuid(_))
        .WillOnce(Return(&pwd))
        .WillOnce(Return(&pwd2))
        .WillOnce(Return(&pwd3));
    std::stringstream ss;
    ss << R"(
noop:x:999:...
test:x:1000:...
test2:x:1001:...
test3:x:1002:...
)";

    auto users = tdm::get_users(ss);
    EXPECT_EQ(users.size(), 1);

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

TEST(passwd, malformed_passwd_file)
{
    std::stringstream ss;
    ss << "abc:def\n";
    EXPECT_THROW(tdm::get_users(ss), std::out_of_range);
}

TEST(passwd, root_user)
{
    EXPECT_NE(sys->getpwuid(0), nullptr);
}
