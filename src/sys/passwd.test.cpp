#include "passwd.h"
#include "../lib/mocks/sys.h"
#include <gtest/gtest.h>

using tdm::sys;
using testing::_;
using testing::Return;

class PasswdTest : public testing::Test
{
  protected:
    std::shared_ptr<tdm::MockSys> m_sys;

  public:
    void SetUp(void)
    {
        m_sys = std::make_shared<tdm::MockSys>();
        sys.set(m_sys);
    }

    void TearDown(void) { sys.reset(); }
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

TEST(passwd, malformed_passwd_file)
{
    std::stringstream ss;
    ss << "abc:def\n";
    EXPECT_THROW(tdm::get_users(ss), std::out_of_range);
}

TEST(passwd, root_user) { EXPECT_NE(sys->getpwuid(0), nullptr); }
