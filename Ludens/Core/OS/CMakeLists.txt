set(MODULE_SRC
	"Include/Error.h"
	"Include/Memory.h"
	"Include/Time.h"
)
set(MODULE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/Ludens")
add_library(LDOS STATIC "${MODULE_SRC}")
target_include_directories(LDOS PRIVATE "${MODULE_INCLUDE_DIR}")
set_target_properties(LDOS PROPERTIES LINKER_LANGUAGE CXX)