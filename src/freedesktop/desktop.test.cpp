#include "desktop.h"
#include "../except.h"
#include "../sys/exec.h"
#include <fstream>
#include <gtest/gtest.h>

using tdm::freedesktop::DesktopFile;

class DesktopTest : public testing::Test
{
  protected:
    std::string tmpdir = "/tmp/tdm-XXXXXX";
    DesktopFile desktop;

  public:
    void SetUp(void) { mkdtemp(tmpdir.data()); }
    void TearDown(void) { std::filesystem::remove_all(tmpdir); }

    using Pair = std::pair<std::string, std::string>;
    std::filesystem::path write_desktop_file(std::vector<Pair> map)
    {
        auto path = std::filesystem::path(tmpdir) / "test.desktop";
        std::ofstream ofs(path.c_str(), std::ios::out);
        ofs << "[Desktop entry]\n";
        for (auto &kv : map) {
            ofs << kv.first << " = " << kv.second << "\n";
        }
        return path;
    }
};

TEST_F(DesktopTest, works)
{
    auto f = write_desktop_file(
        {{"# A skipped comment", ""}, {"Name", "test"}, {"Exec", "test"}});

    // .parse() via path constructor
    EXPECT_NO_THROW({ desktop = DesktopFile(f); });

    // .path()
    EXPECT_EQ(desktop.path(), f.string());

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
    EXPECT_EQ(desktop.parse("test.desktop", content), 4);
}

TEST_F(DesktopTest, path_constructor_nofile_throws)
{
    EXPECT_THROW(DesktopFile("/path/to/nowhere"), std::invalid_argument);
}

TEST_F(DesktopTest, path_constructor_malformed_throws)
{
    auto f = write_desktop_file({{"Name", "test"}, {"Exec", "test"}});
    std::ofstream ofs(f, std::ios::out);
    ofs << "[Desktop entry]\n"
        << "Blah\n";
    ofs.close();
    EXPECT_THROW(DesktopFile d(f), tdm::parse_error);
}

TEST_F(DesktopTest, path_constructor_permission_denied_throws)
{
    auto f = write_desktop_file({{"Name", "test"}, {"Exec", "test"}});

    tdm::Exec chmod("chmod");
    auto args = fmt::format("ug-r {}", f.c_str());
    chmod(args.c_str());
    EXPECT_EQ(chmod.communicate([](auto) {}, [](auto) {}), 0);

    EXPECT_THROW(DesktopFile d(f), std::runtime_error);
}
