#include <iostream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Core/RenderBase/Include/GL/GLContext.h"
#include "Core/RenderBase/Include/GL/GLVertexArray.h"
#include "Core/RenderBase/Include/GL/GLBuffer.h"
#include "Core/RenderBase/Include/GL/GLTexture.h"
#include "Core/RenderBase/Include/GL/GLProgram.h"
#include "Core/Header/Include/Error.h"


namespace LD {

	bool GLContext::sHasGladInit = false;
	GLContextLimits GLContext::sLimits;

	GLContext::GLContext()
	{
	}

	GLContext::~GLContext()
	{
	}

	void GLContext::Setup()
	{
		// lazy initialization in first GLContext setup
		if (!sHasGladInit)
		{
			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			{
				std::cout << "gladLoadGLLoader failed" << std::endl;
				return;
			}

			QueryLimits();
			sHasGladInit = true;
		}

		LD_DEBUG_ASSERT(sHasGladInit);

		GLint major, minor;
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);
		mVersion = (GLuint)(major * 100 + minor * 10);

		glActiveTexture(GL_TEXTURE0);
		mBoundTextureUnit = 0;
	}

	void GLContext::Cleanup()
	{
	}

	/*
	void GLContext::BindVAO(GLVertexArray* vao)
	{
		if (vao == nullptr)
		{
			mBoundVAO = nullptr;
			return;
		}

		if (mBoundVAO != nullptr && (UID)*mBoundVAO == (UID)*vao)
		{
			LD_DEBUG_ASSERT([&]() { GLint actual; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &actual); return actual == (GLuint)*vao; }());
			return;
		}

		glBindVertexArray((GLuint)*vao);
		mBoundVAO = vao;
	}
	*/

	void GLContext::BindVBO(GLVertexBuffer& vbo)
	{
		if (mBoundVBO == (UID)vbo)
		{
			LD_DEBUG_ASSERT([&]() { GLint actual; glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &actual); return actual == (GLuint)vbo; }());
			return;
		}

		glBindBuffer(GL_ARRAY_BUFFER, (GLuint)vbo);
		mBoundVBO = (UID)vbo;
	}

	void GLContext::BindIBO(GLIndexBuffer& ibo)
	{
		LD_DEBUG_ASSERT(mBoundVAO != nullptr && "binding IBO without a previously bound VAO");

		mBoundVAO->BindIBO(ibo);
	}

	void GLContext::BindUBO(GLUniformBuffer& ubo)
	{
		if (mBoundUBO == (UID)ubo)
		{
			LD_DEBUG_ASSERT([&]() { GLint actual; glGetIntegerv(GL_UNIFORM_BUFFER_BINDING, &actual); return actual == (GLuint)ubo; }());
			return;
		}

		glBindBuffer(GL_UNIFORM_BUFFER, (GLuint)ubo);
		mBoundUBO = (UID)ubo;
	}

	/*
	void GLContext::BindTexture2D(GLTexture2D& texture)
	{
		// NOTE: OpenGL texture units supports bindings to all targets, here we don't cache which texture unit we bind to,
		//       only the last texture bound for each texture target.
		glBindTexture(GL_TEXTURE_2D, (GLuint)texture);
		mBoundTexture2D = (UID)texture;
	}

	void GLContext::BindProgram(GLProgram& shader)
	{
		if (mBoundProgram == (UID)shader)
		{
			LD_DEBUG_ASSERT([&]() { GLint actual; glGetIntegerv(GL_CURRENT_PROGRAM, &actual); return actual == (GLuint)shader; }());
			return;
		}

		glUseProgram((GLuint)shader);
		mBoundProgram = (UID)shader;
	}

	void GLContext::BindTextureUnit(int unit)
	{
		if (mBoundTextureUnit == unit)
		{
			LD_DEBUG_ASSERT([&]() { GLint actual; glGetIntegerv(GL_ACTIVE_TEXTURE, &actual); return actual == (GL_TEXTURE0 + unit); }());
			return;
		}
		
		glActiveTexture(GL_TEXTURE0 + unit);
		mBoundTextureUnit = unit;
	}
	*/

	void GLContext::QueryLimits()
	{
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &sLimits.MaxTextureImageUnits);
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &sLimits.MaxCombinedTextureImageUnits);
		glGetIntegerv(GL_MAX_ELEMENT_INDEX, &sLimits.MaxElementIndex);
		glGetIntegerv(GL_MAX_DRAW_BUFFERS, &sLimits.MaxDrawBuffers);
		glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &sLimits.MaxUniformBlockSize);
		glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &sLimits.MaxUniformBufferBindings);
	}


	std::string GLContextLimits::ToString() const
	{
		std::stringstream ss;

		ss << "GL_MAX_TEXTURE_IMAGE_UNITS:\t" << MaxTextureImageUnits << '\n';
		ss << "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS:\t" << MaxCombinedTextureImageUnits << '\n';
		ss << "GL_MAX_ELEMENT_INDEX:\t" << MaxTextureImageUnits << '\n';
		ss << "GL_MAX_DRAW_BUFFERS:\t" << MaxDrawBuffers << '\n';
		ss << "GL_MAX_UNIFORM_BLOCK_SIZE:\t" << MaxUniformBlockSize << '\n';
		ss << "GL_MAX_UNIFORM_BUFFER_BINDINGS:\t" << MaxUniformBufferBindings << '\n';

		return ss.str();
	}

} // namespace LD