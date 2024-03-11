// SPDX-License-Identifier: MIT
#include "testing.h"

namespace tdm::test {

std::filesystem::path mkdtemp(void)
{
    std::string tmpdir("/tmp/tdm-XXXXXX");
    ::mkdtemp(tmpdir.data());
    return tmpdir;
}

TemporaryDirectory::TemporaryDirectory(void)
{
    m_path = mkdtemp();
}

TemporaryDirectory::~TemporaryDirectory(void)
{
    std::filesystem::remove_all(m_path);
}

TemporaryDirectory::operator const std::filesystem::path &(void) const
{
    return m_path;
}

const char *TemporaryDirectory::c_str(void) const
{
    return m_path.c_str();
}

const std::filesystem::path &TemporaryDirectory::path(void) const
{
    return m_path;
}

} // namespace tdm::test
