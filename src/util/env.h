#ifndef UTIL_ENV_H
#define UTIL_ENV_H

#include "../sys/passwd.h"
#include <string>

namespace tdm {
std::string xdg_data_home(const User &user);
};

#endif /* UTIL_ENV_H */
