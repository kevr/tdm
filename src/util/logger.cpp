// SPDX-License-Identifier: MIT
#include "logger.h"
using namespace tdm;

// Default Logger output stream is std::cout
std::ostream *Logger::m_os = &std::cout;
std::ofstream Logger::m_ofs;
std::mutex Logger::m_mutex;
// Verbose logging is disabled by default
bool Logger::m_debug = false;

Logger &Logger::verbose(bool enabled)
{
    m_debug = enabled;
    return *this;
}

bool Logger::open(std::filesystem::path logfile)
{
    m_ofs.close();

    if (logfile.empty()) {
        m_os = &std::cout;
    } else {
        m_ofs.open(logfile, std::ios::out | std::ios::app);
        if (!m_ofs) {
            return false;
        }
        m_os = &m_ofs;
    }

    return true;
}

Logger::Logger(std::string name) : m_name(name)
{
}

std::string Logger::timestamp(void) const
{
    using namespace datetime;
    auto now = system::now();
    return system::strftime(now, "%Y-%m-%d %H:%M:%S %Z");
}
