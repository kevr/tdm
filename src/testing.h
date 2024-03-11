// SPDX-License-Identifier: MIT
#ifndef TESTING_H
#define TESTING_H

#include <filesystem>

namespace tdm::test {

//! Create and return a new temporary directory
std::filesystem::path mkdtemp(void);

class TemporaryDirectory
{
  private:
    std::filesystem::path m_path;

  public:
    TemporaryDirectory(void);
    ~TemporaryDirectory(void);

  public:
    operator const std::filesystem::path &(void) const;
    const char *c_str(void) const;
    const std::filesystem::path &path(void) const;
};

} // namespace tdm::test

#endif /* TESTING_H */
