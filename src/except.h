// SPDX-License-Identifier: MIT
#ifndef EXCEPT_H
#define EXCEPT_H

#include <stdexcept>

namespace tdm {

struct parse_error : public std::domain_error
{
    using std::domain_error::domain_error;
};

}; // namespace tdm

#endif /* EXCEPT_H */
