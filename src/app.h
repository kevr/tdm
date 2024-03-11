// SPDX-License-Identifier: MIT
#ifndef APP_H
#define APP_H

#include "sys/passwd.h"
#include "tui/login.h"
#include <filesystem>
#include <vector>

namespace tdm {

class App
{
  private:
    std::vector<User> m_users;
    tui::Login m_login;

  public:
    int run(const std::filesystem::path &passwd_file);

  private:
    void collect_users(const std::filesystem::path &passwd_file);
};
} // namespace tdm

#endif /* APP_H */
