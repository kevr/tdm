// SPDX-License-Identifier: MIT
#include "env.h"
#include <algorithm>
#include <filesystem>
#include <fmt/format.h>
#include <unistd.h>

namespace tdm {

std::string getenv(const char *var, const std::string &def)
{
    char *result = ::getenv(var);
    return result != nullptr ? result : def;
}

std::string xdg_data_home(const User &user)
{
    return getenv("XDG_DATA_HOME", fmt::format("{}/.local/share", user.home()));
}

} // namespace tdm
