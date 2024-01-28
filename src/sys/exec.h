#ifndef SYS_EXEC_H
#define SYS_EXEC_H

#include "../lib/sys.h"
#include "../util/logger.h"
#include "../util/str.h"
#include <functional>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace tdm {

class Exec
{
  private:
    std::string m_exec;
    std::string m_error;

    FILE *m_stdout = nullptr;
    FILE *m_stderr = nullptr;

    int m_pfd[2] = {0, 0};  // Pipe fds
    std::vector<int> m_fds; // List of fds to include during select()
    std::map<int, std::stringstream> m_streams;
    std::map<int, size_t> m_lines;

    std::map<int, std::function<void(void)>>
        m_closures; // Mapping of fd -> FILE *
    std::map<int, std::function<void(void)>> m_actions;

    int m_return_code = -1;

  public:
    Exec(std::string exec);
    ~Exec(void);

  public:
    int return_code(void) const;
    const std::string &error(void) const;

    int operator()(const char *args);

    int communicate(std::function<void(std::string)> stdout_fn,
                    std::function<void(std::string)> stderr_fn);

  private:
    int set_nonblocking(int fd);
    int read_some(int fd, std::stringstream &buffer, size_t &lines);

    int set_error(int error);

    int close(void);
};
} // namespace tdm

#endif /* SYS_EXEC_H */
