// SPDX-License-Identifier: MIT
#ifndef LIB_TERM_H
#define LIB_TERM_H

#include "../singleton.h"

namespace tdm::lib {

class Term
{
  public:
    virtual ~Term(void) = default;
    virtual int tigetnum(const char *name);
};

inline Singleton<Term> term;
} // namespace tdm::lib

#endif /* LIB_TERM_H */
