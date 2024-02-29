// SPDX-License-Identifier: MIT
#include "session.h"
#include "../config.h"
#include "../lib/mocks/pam.h"
#include "../lib/mocks/sys.h"
#include <functional>
#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <vector>

using namespace tdm;
using lib::MockPam;
using testing::_;
using testing::Invoke;
using testing::Return;
using namespace std::placeholders;

class SessionTest : public testing::Test
{
  protected:
    static std::vector<std::function<void(void)>> m_mocks;

    std::shared_ptr<MockSys> m_sys;
    std::shared_ptr<MockPam> m_pam;

    pam_handle_t *m_handle;
    struct passwd pwd;

  public:
    void SetUp(void)
    {
        logger.verbose(true);

        m_sys = std::make_shared<MockSys>();
        sys.set(m_sys);

        m_pam = std::make_shared<MockPam>();
        lib::pam.set(m_pam);

        EXPECT_CALL(*m_sys, getpwnam(_))
            .WillOnce(Invoke(std::bind(&Sys::getpwnam, *sys, _1)));
    }

    void TearDown(void)
    {
        lib::pam.reset();
        sys.reset();
    }

  protected:
    void add_mock(std::function<void(void)> mock_fn)
    {
        m_mocks.emplace_back(mock_fn);
    }

    void run_mocks(void)
    {
        for (auto &mock : m_mocks) {
            mock();
        }
    }
};

std::vector<std::function<void(void)>> SessionTest::m_mocks;

pam::Session expect_login(const std::string &user, const std::string &password,
                          int return_code)
{
    pam::Session session(user);
    EXPECT_EQ(session.login(password), return_code);
    return session;
} // LCOV_EXCL_LINE

TEST_F(SessionTest, start_fails)
{
    EXPECT_CALL(*m_pam, start_confdir(_, _, _, _, _))
        .WillOnce(Return(PAM_SYSTEM_ERR));
    expect_login(TEST_USER, TEST_PASS, PAM_SYSTEM_ERR);
}

TEST_F(SessionTest, authenticate_fails)
{
    add_mock([this] {
        // Forward MockPam::start to lib::pam->start(a, b, c, d)
        EXPECT_CALL(*m_pam, start_confdir(_, _, _, _, _))
            .WillOnce(Invoke(std::bind(&lib::Pam::start_confdir, *lib::pam, _1,
                                       _2, _3, _4, _5)));
    });
    run_mocks();
    EXPECT_CALL(*m_pam, authenticate(_, _)).WillOnce(Return(PAM_AUTH_ERR));

    expect_login(TEST_USER, TEST_PASS, PAM_AUTH_ERR);
}

TEST_F(SessionTest, open_session_fails)
{
    add_mock([this] {
        EXPECT_CALL(*m_pam, authenticate(_, _)).WillOnce(Return(PAM_SUCCESS));
    });
    run_mocks();
    EXPECT_CALL(*m_pam, open_session(_, _)).WillOnce(Return(PAM_SESSION_ERR));

    expect_login(TEST_USER, TEST_PASS, PAM_SESSION_ERR);
}

TEST_F(SessionTest, seteuid_fails)
{
    add_mock([this] {
        EXPECT_CALL(*m_pam, open_session(_, _)).WillOnce(Return(PAM_SUCCESS));
    });
    run_mocks();
    EXPECT_CALL(*m_sys, seteuid(_)).WillOnce(Return(-1));

    expect_login(TEST_USER, TEST_PASS, -1);
}

TEST_F(SessionTest, works)
{
    add_mock([this] {
        EXPECT_CALL(*m_sys, seteuid(_)).WillOnce(Return(0));
        EXPECT_CALL(*m_sys, setegid(_)).WillOnce(Return(0));
    });
    add_mock([this] {
        EXPECT_CALL(*m_pam, close_session(_, _)).WillOnce(Return(PAM_SUCCESS));

        // Forward MockPam::end to lib::pam->end(a, b)
        EXPECT_CALL(*m_pam, end(_, _))
            .WillOnce(Invoke(std::bind(&lib::Pam::end, *lib::pam, _1, _2)));
    });
    run_mocks();

    auto session = expect_login(TEST_USER, TEST_PASS, PAM_SUCCESS);
    EXPECT_EQ(session.user(), TEST_USER);
}

TEST(PamSession, non_existent_user)
{
    expect_login("non-existent-user", "fake", -1);
}

TEST(PamSession, test_user)
{
    // Test real unmocked real PAM implementation
    expect_login(TEST_USER, TEST_PASS, PAM_SUCCESS);
}
