#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

#include "Core/Header/Include/Types.h"
#include "Core/Header/Include/Error.h"


// Sanity Check
LD_STATIC_ASSERT(sizeof(LD::u8) == 1);
LD_STATIC_ASSERT(sizeof(LD::u16) == 2);
LD_STATIC_ASSERT(sizeof(LD::u32) == 4);
LD_STATIC_ASSERT(sizeof(LD::u64) == 8);
