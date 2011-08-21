#include "lassert.h"

const lexcept_t assert_failed = { "Assertion failed" };

void (lassert)(int e){
	assert(e);
}
