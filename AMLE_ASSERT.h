#pragma once

#include <cassert>





#ifdef _DEBUG
#define AMLE_ASSERT(x) assert(x)
#else
#define AMLE_ASSERT(x)
#endif
