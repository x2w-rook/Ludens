#include "Core/RenderBase/Include/GL/GLCommand.h"

namespace LD {
namespace GLCommand {

	void DrawArrays(GLenum primitive, u32 count, int firstVertex)
	{
		glDrawArrays(primitive, firstVertex, count);
	}

	void DrawElements(GLenum primitive, u32 count, GLenum indexType)
	{
		glDrawElements(primitive, count, indexType, nullptr);
	}

} // namespace GLCommand
} // namespace LD