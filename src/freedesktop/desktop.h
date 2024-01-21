#ifndef FREEDESKTOP_DESKTOP_H
#define FREEDESKTOP_DESKTOP_H

#include <map>
#include <string>
#include <string_view>

namespace tdm::freedesktop {

class DesktopFile
{
  private:
    std::map<std::string, std::string> m_options;

  public:
    DesktopFile(void) = default;
    DesktopFile(const DesktopFile &o);
    DesktopFile(DesktopFile &&o);

  public:
    DesktopFile &operator=(const DesktopFile &o);
    DesktopFile &operator=(DesktopFile &&o);

  public:
    /**
     * @brief Parse the string content of a .desktop file
     *
     * @returns On success, 0 is returned.
     *          On failure, the malformed line number is returned.
     **/
    std::size_t parse(std::string_view sv);

    std::string get(const std::string &key) const;
};

}; // namespace tdm::freedesktop

#endif /* FREEDESKTOP_DESKTOP_H */
