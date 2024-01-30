#ifndef APP_H
#define APP_H

#include "sys/passwd.h"
#include <filesystem>
#include <string>
#include <vector>

namespace tdm {
class App
{
  private:
    bool m_init = false;
    std::vector<tdm::User> m_users;

  public:
    App(void);
    ~App(void);

  public:
    int run(const std::filesystem::path &passwd_file);

  private:
    void collect_users(const std::filesystem::path &passwd_file);
};
} // namespace tdm

#endif /* APP_H */
