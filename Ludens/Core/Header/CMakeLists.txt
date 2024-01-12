set(MODULE_SRC
	"Include/Platform.h"
	"Include/Error.h"
	"Include/Types.h")

set(TEST_SRC
	"Tests/HeaderTests.cpp")

add_executable(LDHeaderTests "${MODULE_SRC}" "${TEST_SRC}")
target_include_directories(LDHeaderTests PRIVATE
	"${CMAKE_SOURCE_DIR}/Ludens"
	"${CMAKE_SOURCE_DIR}/Extra/doctest")