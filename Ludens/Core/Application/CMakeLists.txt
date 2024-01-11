set(MODULE_SRC
	"Include/Application.h"
	"Include/ApplicationLayer.h"
	"Include/Event.h"
	"Include/Input.h"
	"Lib/Application.cpp"
	"Lib/ApplicationWindow.h"
	"Lib/ApplicationWindow.cpp"
	"Lib/Event.cpp"
	"Lib/Input.cpp"
)
set(MODULE_INCLUDE_DIR
	"${CMAKE_SOURCE_DIR}/Ludens"
	"${CMAKE_SOURCE_DIR}/Extra/glfw/include"
)
add_library(LDApplication STATIC "${MODULE_SRC}")
target_include_directories(LDApplication PRIVATE "${MODULE_INCLUDE_DIR}")
target_link_libraries(LDApplication PRIVATE glfw)