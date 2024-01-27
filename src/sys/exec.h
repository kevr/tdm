#ifndef SYS_EXEC_H
#define SYS_EXEC_H

#include "../lib/sys.h"
#include "../util/logger.h"
#include "../util/str.h"
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <format>
#include <functional>
#include <string>
#include <sys/select.h>
#include <unistd.h>
#include <utility>
#include <vector>

namespace tdm {

//! Read a string line from fd
ssize_t getline(FILE *fd, std::string &line);

class Exec
{
  private:
    std::string m_exec;
    std::string m_error;

    FILE *m_stdout = nullptr;
    FILE *m_stderr = nullptr;

    int m_return_code = -1;

    std::vector<int> select_fds;
    int pfd[2] = {0, 0};

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
    int set_error(int error);
    int set_nonblocking(int fd);

    int select(std::function<void(FILE *)> stdout_fn,
               std::function<void(FILE *)> stderr_fn);

    int close(void);
};
} // namespace tdm

#endif /* SYS_EXEC_H */
