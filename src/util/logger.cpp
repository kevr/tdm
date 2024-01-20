#include "logger.h"
using namespace tdm;

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

Logger::Logger(std::string name) : m_name(name) {}
