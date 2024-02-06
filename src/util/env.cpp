#include "env.h"
#include <algorithm>
#include <filesystem>
#include <fmt/format.h>
#include <unistd.h>

namespace tdm {
std::string xdg_data_home(const User &user)
{
    const char *path = getenv("XDG_DATA_HOME");
    if (path) {
        return path;
    }

    return fmt::format("{}/.local/share", user.home());
}
} // namespace tdm
