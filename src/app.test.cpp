// SPDX-License-Identifier: MIT
#include "app.h"
#include "lib/mocks/curses.h"
#include "lib/mocks/sys.h"
#include "testing.h"
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <memory>

using namespace tdm;
using namespace tdm::lib;
using testing::_;
using testing::NiceMock;
using testing::Return;

class AppTest : public testing::Test
{
  protected:
    std::shared_ptr<MockSys> m_sys = std::make_shared<MockSys>();
    std::shared_ptr<NiceMock<MockCurses>> m_curses =
        std::make_shared<NiceMock<MockCurses>>();

    test::TemporaryDirectory tmpdir;
    std::filesystem::path passwd = tmpdir.path() / "passwd";

    WINDOW root;
    WINDOW child;

    struct passwd pwd1, pwd2;

  public:
    void SetUp(void)
    {
        sys.set(m_sys);
        curses.set(m_curses);
        setenv("XDG_DATA_HOME", tmpdir.c_str(), 1);
    }

    void TearDown(void)
    {
        unsetenv("XDG_DATA_HOME");
        curses.reset();
        sys.reset();
    }

  protected:
    void mock_initscr(void)
    {
        EXPECT_CALL(*m_curses, initscr()).WillOnce(Return(&root));
        EXPECT_CALL(*m_curses, endwin()).WillOnce(Return(OK));
    }

    void mock_derwin(void)
    {
        EXPECT_CALL(*m_curses, derwin(_, _, _, _, _))
            .WillRepeatedly(Return(&child));
        EXPECT_CALL(*m_curses, delwin(_)).WillRepeatedly(Return(OK));
    }

    void mock_getchar(void)
    {
        EXPECT_CALL(*m_curses, getchar()).WillOnce(Return('q'));
    }

  protected:
    struct passwd make_passwd(const char *name, uid_t uid, gid_t gid,
                              const char *dir, const char *shell)
    {
        return {.pw_name = const_cast<char *>(name),
                .pw_passwd = nullptr,
                .pw_uid = uid,
                .pw_gid = gid,
                .pw_gecos = nullptr,
                .pw_dir = const_cast<char *>(dir),
                .pw_shell = const_cast<char *>(shell)};
    }

    void make_passwd_file(const std::filesystem::path &path)
    {
        std::ofstream ofs(path, std::ios::out);
        ofs << "test:x:1000:1000:...\n"
            << "test2:x:1001:1001:...\n";
    }

    void prepare(const std::filesystem::path &tmpdir)
    {
        make_passwd_file(tmpdir / "passwd");

        pwd1 = make_passwd("test", 1000, 1000, "/home/test", "/usr/bin/bash");
        pwd2 = make_passwd("test2", 1001, 1001, "/home/test2", "/usr/bin/bash");
        EXPECT_CALL(*m_sys, getpwuid(_))
            .WillOnce(Return(&pwd1))
            .WillOnce(Return(&pwd2));

        auto xsessions = tmpdir / "xsessions";
        mkdir(xsessions.c_str(), 0755);

        auto desktop = xsessions / "test.desktop";
        std::ofstream ofs(desktop, std::ios::out);
        ofs << "[Desktop entry]\n"
            << "Name = test\n"
            << "Exec = test\n";
        ofs.close();
    }
};

TEST_F(AppTest, succeeds)
{
    prepare(tmpdir);

    mock_initscr();
    mock_derwin();
    mock_getchar();

    EXPECT_EQ(App().run(passwd), 0);
}

TEST_F(AppTest, login_init_fails)
{
    prepare(tmpdir);

    // NiceMock will return nullptr from initscr() by default,
    // so test that running without a mock results in error.
    EXPECT_EQ(App().run(passwd), ERR);
}

TEST_F(AppTest, KEY_RESIZE)
{
    prepare(tmpdir);

    mock_initscr();
    EXPECT_CALL(*m_curses, derwin(_, _, _, _, _))
        .WillRepeatedly(Return(&child));
    EXPECT_CALL(*m_curses, getchar())
        .WillOnce(Return(KEY_RESIZE))
        .WillOnce(Return('q'));

    EXPECT_EQ(App().run(passwd), 0);
}

TEST_F(AppTest, getchar_error)
{
    prepare(tmpdir);

    mock_initscr();
    mock_derwin();
    EXPECT_CALL(*m_curses, getchar()).WillOnce(Return(-1));

    EXPECT_EQ(App().run(passwd), -1);
}
