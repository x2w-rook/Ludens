set(MODULE_SRC
	"Include/Array.h"
	"Include/Vector.h"
	"Include/Optional.h")

set(TEST_SRC
	"Tests/TestVector.h"
	"Tests/TestArray.h"
	"Tests/TestOptional.h"
	"Tests/DSATests.cpp")

add_executable(LDDSATests "${MODULE_SRC}" "${TEST_SRC}")
target_include_directories(LDDSATests PRIVATE
	"${CMAKE_SOURCE_DIR}/Ludens"
	"${CMAKE_SOURCE_DIR}/Extra/doctest")

target_link_libraries(LDDSATests LDOS)