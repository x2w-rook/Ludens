#pragma once

#include <vector>
#include <cstdint>
#include <glad/glad.h>
#include "Core/Header/Include/Types.h"
#include "Core/RenderBase/Include/GL/GLSL.h"
#include "Core/DSA/Include/Vector.h"

namespace LD {

	struct GLVertexAttribute
	{
		GLuint Location;
		GLSLType Type;
		GLenum Normalize = GL_FALSE;
	};

	class GLVertexLayout
	{
	public:
		GLVertexLayout& AddVertexAttribute(const GLVertexAttribute& attribute);
		inline const Vector<GLVertexAttribute>& GetAttributes() const { return mAttributes; }

	private:
		u32 mVertexStride = 0;
		Vector<GLVertexAttribute> mAttributes;
		Vector<u32> mOffsets;
	};

} // namespace LD