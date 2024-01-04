#pragma once


// Debug Assertions
// - the asserted expression will be stripped in release builds
// - mostly used to assert prerequisites and crash early in debug builds
#ifndef LD_DEBUG_ASSERT
# include <cassert>
# define LD_DEBUG_ASSERT(EXPR)       assert(EXPR)
#endif