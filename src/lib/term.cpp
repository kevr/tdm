// SPDX-License-Identifier: MIT
#include "term.h"
#include <term.h>

namespace tdm::lib {

int Term::tigetnum(const char *name)
{
    return ::tigetnum(name);
}

} // namespace tdm::lib
