#ifndef UTIL_LOGGER_H
#define UTIL_LOGGER_H

#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>

namespace tdm {
class Logger
{
  private:
    inline static std::ostream *m_os = &std::cout;
    inline static std::ofstream m_ofs;
    inline static std::mutex m_mutex;

  private:
    std::string m_name;

  public:
    static bool open(std::filesystem::path logfile = std::filesystem::path());

  public:
    Logger(std::string name);

  public:
    template <typename... Args>
    int info(std::format_string<Args...> fmt, Args &&...args)
    {
        return write("INFO", std::move(fmt), std::forward<Args>(args)...);
    }

    template <typename... Args>
    int warning(std::format_string<Args...> fmt, Args &&...args)
    {
        write("WARNING", std::move(fmt), std::forward<Args>(args)...);
        return 1;
    }

    template <typename... Args>
    int error(std::format_string<Args...> fmt, Args &&...args)
    {
        write("ERROR", std::move(fmt), std::forward<Args>(args)...);
        return 1;
    }

  private:
    template <typename... Args>
    int write(const std::string &label, std::format_string<Args...> fmt,
              Args &&...args)
    {
        std::lock_guard<std::mutex> _(m_mutex);
        *m_os << std::format("[{}:{}] {}\n", label, m_name,
                             std::format(fmt, std::forward<Args>(args)...));
        m_os->flush();
        return 0;
    }
};

inline Logger logger("root");
}; // namespace tdm

#endif /* UTIL_LOGGER_H */
