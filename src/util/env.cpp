#include "env.h"
#include <algorithm>
#include <filesystem>
#include <format>
#include <unistd.h>

namespace tdm {
std::string xdg_data_home(void)
{
    const char *path = getenv("XDG_DATA_HOME");
    if (path) {
        return path;
    }

    const char *home = getenv("HOME");
    return std::format("{}/.local/share", home);
}
} // namespace tdm
