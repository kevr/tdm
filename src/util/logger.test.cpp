#include "logger.h"
#include "str.h"
#include "gtest/gtest.h"
#include <gtest/gtest.h>
#include <regex>

using tdm::logger;
using testing::internal::CaptureStdout;
using testing::internal::GetCapturedStdout;

class LoggerTest : public testing::Test
{
  public:
    void SetUp(void)
    {
        logger.verbose(true);
        CaptureStdout();
    }

    void TearDown(void)
    {
        logger.verbose(false);
    }
};

TEST_F(LoggerTest, info)
{
    logger.info("test");
    auto output = GetCapturedStdout();
    EXPECT_NE(output.rfind("[INFO:root] test\n"), std::string::npos);
}

TEST_F(LoggerTest, timestamp)
{
    logger.info("test");
    auto output = GetCapturedStdout();
    output = tdm::rstrip(output);

    std::regex re(R"(^\[\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2} .{3}\] .*$)");
    std::smatch match;
    EXPECT_TRUE(std::regex_match(output, match, re));
}

TEST_F(LoggerTest, debug)
{
    logger.debug("test");
    auto output = GetCapturedStdout();
    EXPECT_NE(output.rfind("[DEBUG:root] test\n"), std::string::npos);
}

TEST_F(LoggerTest, warning)
{
    logger.warning("test");
    auto output = GetCapturedStdout();
    EXPECT_NE(output.rfind("[WARNING:root] test\n"), std::string::npos);
}

TEST_F(LoggerTest, error)
{
    logger.error("test");
    auto output = GetCapturedStdout();
    EXPECT_NE(output.rfind("[ERROR:root] test\n"), std::string::npos);
}

TEST_F(LoggerTest, open_gracefully_fails)
{
    EXPECT_FALSE(logger.open("/sys/fake"));
    GetCapturedStdout();
}

TEST(logger, debug_noop)
{
    // When logger.verbose() is not used to enable verbosity,
    // Logger::debug should noop.
    CaptureStdout();
    logger.debug("test");
    auto output = GetCapturedStdout();
    EXPECT_EQ(output, "");
}

TEST(logger, open_resets)
{
    EXPECT_TRUE(logger.open());
}
