#pragma once

#include <glad/glad.h>
#include "Core/Header/Include/Types.h"

namespace LD {
namespace GLCommand {

	void DrawArrays(GLenum primitive, u32 count, int firstVertex = 0);
	
	void DrawElements(GLenum primitive, u32 count, GLenum indexType);

} // namespace GLCommand
} // namespace LD