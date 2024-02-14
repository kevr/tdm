#ifndef UTIL_LOGGER_H
#define UTIL_LOGGER_H

#include "datetime.h"
#include <chrono>
#include <filesystem>
#include <fmt/format.h>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>

namespace tdm {
class Logger
{
  private:
    static std::ostream *m_os;
    static std::ofstream m_ofs;
    static std::mutex m_mutex;
    static bool m_debug;

  private:
    std::string m_name;

  public:
    static bool open(std::filesystem::path logfile = std::filesystem::path());

  public:
    Logger(std::string name);

  public:
    template <typename... Args>
    int info(fmt::format_string<Args...> fmt, Args &&...args)
    {
        return write("INFO", std::move(fmt), std::forward<Args>(args)...);
    }

    Logger &verbose(bool enabled);

    template <typename... Args>
    int debug(fmt::format_string<Args...> fmt, Args &&...args)
    {
        return m_debug
                   ? write("DEBUG", std::move(fmt), std::forward<Args>(args)...)
                   : 0;
    }

    template <typename... Args>
    int warning(fmt::format_string<Args...> fmt, Args &&...args)
    {
        write("WARNING", std::move(fmt), std::forward<Args>(args)...);
        return 1;
    }

    template <typename... Args>
    int error(fmt::format_string<Args...> fmt, Args &&...args)
    {
        write("ERROR", std::move(fmt), std::forward<Args>(args)...);
        return 1;
    }

  private:
    template <typename... Args>
    int write(const std::string &label, fmt::format_string<Args...> fmt,
              Args &&...args)
    {
        std::lock_guard<std::mutex> _(m_mutex);
        *m_os << fmt::format("[{}] [{}:{}] {}\n", timestamp(), label, m_name,
                             fmt::format(fmt, std::forward<Args>(args)...));
        m_os->flush();
        return 0;
    }

    std::string timestamp(void) const;
};

inline Logger logger("root");
}; // namespace tdm

#endif /* UTIL_LOGGER_H */
