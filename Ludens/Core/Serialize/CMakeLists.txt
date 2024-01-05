set(MODULE_SRC
	"Include/INI.h"
	"Lib/INI.cpp"
)
set(MODULE_INCLUDE_DIR
	"${CMAKE_SOURCE_DIR}/Ludens"
	"${CMAKE_SOURCE_DIR}/Extra/doctest"
)
add_library(LDSerialize STATIC "${MODULE_SRC}")
target_include_directories(LDSerialize PRIVATE "${MODULE_INCLUDE_DIR}")

set(TEST_SRC
	"Tests/SerializeTests.cpp"
	"Tests/SerializeINI.h"
)
add_executable(LDSerializeTests "${MODULE_SRC}" "${TEST_SRC}")
target_include_directories(LDSerializeTests PRIVATE "${MODULE_INCLUDE_DIR}")