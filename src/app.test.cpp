#include "app.h"
#include "lib/mocks/sys.h"
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <memory>

using namespace tdm;
using testing::_;
using testing::Return;

class AppTest : public testing::Test
{
  protected:
    std::shared_ptr<MockSys> m_sys = std::make_shared<MockSys>();
    std::string tmpdir = "/tmp/tdm-XXXXXX";

  public:
    void SetUp(void)
    {
        mkdtemp(tmpdir.data());
        sys.set(m_sys);
    }
    void TearDown(void)
    {
        sys.reset();
        std::filesystem::remove_all(tmpdir);
    }
};

TEST_F(AppTest, with_users)
{
    std::filesystem::path tmp(tmpdir);
    auto passwd = tmp / "passwd";
    {
        std::ofstream ofs(passwd);
        ofs << "test:x:1000:1000:...\n"
            << "test2:x:1001:1001:...\n";
    }

    struct passwd pwd1 = {.pw_name = const_cast<char *>("test"),
                          .pw_passwd = nullptr,
                          .pw_uid = 1000,
                          .pw_gid = 1000,
                          .pw_gecos = nullptr,
                          .pw_dir = const_cast<char *>("/home/test"),
                          .pw_shell = const_cast<char *>("/usr/bin/bash")};
    struct passwd pwd2 = {.pw_name = const_cast<char *>("test2"),
                          .pw_passwd = nullptr,
                          .pw_uid = 1001,
                          .pw_gid = 1001,
                          .pw_gecos = nullptr,
                          .pw_dir = const_cast<char *>("/home/test2"),
                          .pw_shell = const_cast<char *>("/usr/bin/bash")};
    EXPECT_CALL(*m_sys, getpwuid(_))
        .WillOnce(Return(&pwd1))
        .WillOnce(Return(&pwd2));
    App().run(passwd);
}
