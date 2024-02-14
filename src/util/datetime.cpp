#include "datetime.h"
#include <iomanip>
#include <sstream>

namespace tdm::datetime {

std::chrono::system_clock::time_point system::now(void)
{
    return std::chrono::system_clock::now();
}

std::string system::strftime(const std::chrono::system_clock::time_point &tp,
                             const char *const fmt)
{
    const auto time_t_ = std::chrono::system_clock::to_time_t(tp);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t_), fmt);
    return ss.str();
}

} // namespace tdm::datetime
