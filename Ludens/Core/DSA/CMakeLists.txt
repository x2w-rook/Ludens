set(MODULE_SRC
	"Include/Array.h")

set(TEST_SRC
	"Tests/TestArray.h"
	"Tests/DSATests.cpp")

add_executable(LDDSATests "${MODULE_SRC}" "${TEST_SRC}")
target_include_directories(LDDSATests PRIVATE
	"${CMAKE_SOURCE_DIR}/Ludens"
	"${CMAKE_SOURCE_DIR}/Extra/doctest")