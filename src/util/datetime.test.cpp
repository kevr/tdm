// SPDX-License-Identifier: MIT
#include "datetime.h"
#include <gtest/gtest.h>

using namespace tdm::datetime;

TEST(datetime, strftime)
{
    const char *const fmt = "%Y-%m-%d %H:%M:%S %Z";

    auto now = system::now();
    auto date_str = system::strftime(now, fmt);

    const auto time_t_ = std::chrono::system_clock::to_time_t(now);
    std::string target_str;
    {
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t_), fmt);
        target_str = ss.str();
    }

    // Expect that our strftime output was equivalent
    // to chrono's std::put_time
    EXPECT_EQ(date_str, target_str);
}
