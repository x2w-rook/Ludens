#pragma once


// Debug Assertions
// - the asserted expression will be stripped in release builds
// - mostly used to assert prerequisites and crash early in debug builds
#ifndef LD_DEBUG_ASSERT
# include <cassert>
# define LD_DEBUG_ASSERT(EXPR)       assert(EXPR)
#endif


// Static Assertions
// - the asserted expression is checked by the compiler at compile time
// - does not need to be in function body
#ifndef LD_STATIC_ASSERT
# define LD_STATIC_ASSERT(EXPR)      static_assert(EXPR, "")
#endif