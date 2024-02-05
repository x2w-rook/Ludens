set(MODULE_SRC
	"Include/Memory.h"
	"Include/Allocator.h"
	"Include/Time.h"
	"Include/UID.h"
	"Lib/Memory.cpp")

set(TEST_SRC
	"Tests/TestPoolAllocator.h"
	"Tests/TestMemory.h"
	"Tests/TestUID.h"
	"Tests/OSTests.cpp")

set(MODULE_INCLUDE_DIR
	"${CMAKE_SOURCE_DIR}/Ludens")

add_library(LDOS STATIC
	"${MODULE_SRC}")

target_include_directories(LDOS PRIVATE
	"${MODULE_INCLUDE_DIR}")

add_executable(LDOSTests
	"${MODULE_SRC}"
	"${TEST_SRC}")

target_include_directories(LDOSTests PRIVATE
	"${CMAKE_SOURCE_DIR}/Ludens"
	"${CMAKE_SOURCE_DIR}/Extra/doctest")