#ifndef FREEDESKTOP_DESKTOP_H
#define FREEDESKTOP_DESKTOP_H

#include <filesystem>
#include <map>
#include <string>
#include <string_view>

namespace tdm::freedesktop {

class DesktopFile
{
  private:
    std::filesystem::path m_path;
    std::map<std::string, std::string> m_options;

  public:
    DesktopFile(void) = default;
    DesktopFile(const std::filesystem::path &path);
    DesktopFile(const DesktopFile &o);
    DesktopFile(DesktopFile &&o);

  public:
    DesktopFile &operator=(const DesktopFile &o);
    DesktopFile &operator=(DesktopFile &&o);

  public:
    const std::filesystem::path &path(void) const;

    /**
     * @brief Parse the string content of a .desktop file
     *
     * @param path Pathing metadata about the .desktop file's location
     * @param content String content of a .desktop file
     *
     * @returns On success, 0 is returned.
     *          On failure, the malformed line number is returned.
     **/
    std::size_t parse(const std::filesystem::path &path,
                      std::string_view content);

    const std::string &get(const std::string &key) const;
};

}; // namespace tdm::freedesktop

#endif /* FREEDESKTOP_DESKTOP_H */
