set(MODULE_INCLUDE
	"Include/GL/GL.h"
	"Include/GL/GLSL.h"
	"Include/GL/GLContext.h"
	"Include/GL/GLCommand.h"
	"Include/GL/GLVertexArray.h"
	"Include/GL/GLVertex.h"
	"Include/GL/GLTexture.h"
	"Include/GL/GLBuffer.h"
	"Include/GL/GLProgram.h")

set(MODULE_LIB
	"Lib/GL/GLSL.cpp"
	"Lib/GL/GLContext.cpp"
	"Lib/GL/GLCommand.cpp"
	"Lib/GL/GLVertexArray.cpp"
	"Lib/GL/GLVertex.cpp"
	"Lib/GL/GLTexture.cpp"
	"Lib/GL/GLBuffer.cpp"
	"Lib/GL/GLProgram.cpp")

set(MODULE_INCLUDE_DIR
	"${CMAKE_SOURCE_DIR}/Ludens"
	"${CMAKE_SOURCE_DIR}/Extra/glad/include"
	"${CMAKE_SOURCE_DIR}/Extra/glfw/include")

add_library(LDRenderBase STATIC "${MODULE_INCLUDE}" "${MODULE_LIB}")
target_include_directories(LDRenderBase PRIVATE "${MODULE_INCLUDE_DIR}")
target_link_libraries(LDRenderBase PRIVATE glad)
