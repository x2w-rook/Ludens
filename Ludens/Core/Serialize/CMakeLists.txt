set(MODULE_LIB
	"Lib/INI.cpp"
	"Lib/XML.cpp"
)

set(MODULE_INCLUDE
	"Include/INI.h"
	"Include/XML.h"
)

set(MODULE_INCLUDE_DIR
	"${CMAKE_SOURCE_DIR}/Ludens"
	"${CMAKE_SOURCE_DIR}/Extra/doctest"
)

add_library(LDSerialize STATIC
	"${MODULE_LIB}"
	"${MODULE_INCLUDE}"
)

target_include_directories(LDSerialize PRIVATE "${MODULE_INCLUDE_DIR}")

set(TEST_SRC
	"Tests/SerializeTests.cpp"
	"Tests/SerializeINI.h"
	"Tests/SerializeXML.h"
)

add_executable(LDSerializeTests "${TEST_SRC}")
target_include_directories(LDSerializeTests PRIVATE "${MODULE_INCLUDE_DIR}")
target_link_libraries(LDSerializeTests PRIVATE
	LDSerialize
	LDOS
)