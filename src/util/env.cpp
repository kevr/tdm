// SPDX-License-Identifier: MIT
#include "env.h"
#include <filesystem>
#include <fmt/format.h>
#include <unistd.h>

namespace tdm {

std::string getenv(const char *var, const std::string &def)
{
    char *result = ::getenv(var);
    return result != nullptr ? result : def;
}

std::filesystem::path xdg_data_home(void)
{
    return getuid() ? fmt::format("{}/.local/share", ::getenv("HOME"))
                    : "/usr/share";
}

std::string xdg_data_home(const User &user)
{
    return getenv("XDG_DATA_HOME", fmt::format("{}/.local/share", user.home()));
}

std::filesystem::path tdm_data_dir(void)
{
    return xdg_data_home() / "tdm";
}

std::filesystem::path tdm_log_dir(void)
{
    return getuid() ? tdm_data_dir() : "/var/log";
}

} // namespace tdm
