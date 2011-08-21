#ifndef LASSERT_H
#define LASSERT_H

#ifdef NDEBUG
#define assert(e) ((void)0)
#else
#include "lexcept.h"
#define assert(e) ((void)((e) || (RAISE(assert_failed), 0)))
#endif

extern const lexcept_t assert_failed;
#endif
