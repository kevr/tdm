#include "logger.h"
#include "gtest/gtest.h"
#include <gtest/gtest.h>

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

    void TearDown(void) { logger.verbose(false); }
};

TEST_F(LoggerTest, info)
{
    logger.info("test");
    auto output = GetCapturedStdout();
    EXPECT_EQ(output, "[INFO:root] test\n");
}

TEST_F(LoggerTest, debug)
{
    logger.debug("test");
    auto output = GetCapturedStdout();
    EXPECT_EQ(output, "[DEBUG:root] test\n");
}

TEST_F(LoggerTest, warning)
{
    logger.warning("test");
    auto output = GetCapturedStdout();
    EXPECT_EQ(output, "[WARNING:root] test\n");
}

TEST_F(LoggerTest, error)
{
    logger.error("test");
    auto output = GetCapturedStdout();
    EXPECT_EQ(output, "[ERROR:root] test\n");
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

TEST(logger, open_resets) { EXPECT_TRUE(logger.open()); }
