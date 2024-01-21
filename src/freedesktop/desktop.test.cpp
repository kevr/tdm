#include "desktop.h"
#include <gtest/gtest.h>

using tdm::freedesktop::DesktopFile;

class DesktopTest : public testing::Test
{
  protected:
    DesktopFile desktop;
};

TEST_F(DesktopTest, works)
{
    std::string content(R"(
# A skipped comment
[Desktop Entry]
Name=test
Exec=test
)");

    // .parse()
    EXPECT_EQ(desktop.parse(content), 0);

    // .get()
    EXPECT_EQ(desktop.get("Name"), "test");
    EXPECT_EQ(desktop.get("Exec"), "test");

    // copy construction
    DesktopFile desktop_copy(desktop);
    EXPECT_EQ(desktop_copy.get("Name"), desktop.get("Name"));

    // copy assignment
    DesktopFile desktop_copy_assigned;
    desktop_copy_assigned = desktop;
    EXPECT_EQ(desktop_copy_assigned.get("Name"), desktop.get("Name"));

    // move construction
    DesktopFile desktop_move(std::move(desktop_copy));
    EXPECT_EQ(desktop_move.get("Name"), desktop.get("Name"));

    // move assignment
    DesktopFile desktop_move_assigned;
    desktop_move_assigned = std::move(desktop_move);
    EXPECT_THROW(desktop_move.get("Name"), std::out_of_range);
    EXPECT_EQ(desktop_move_assigned.get("Name"), desktop.get("Name"));
}

TEST_F(DesktopTest, unknown_key)
{
    EXPECT_THROW(desktop.get("Fake"), std::out_of_range);
}

TEST_F(DesktopTest, malformed_line)
{
    std::string content(R"(
[Desktop Entry]
Name=test
Exec
)");
    EXPECT_EQ(desktop.parse(content), 4);
}
