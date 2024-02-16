#pragma once

#include <string>
#include <glad/glad.h>
#include "Core/OS/Include/UID.h"

namespace LD {

	class GLVertexArray;
	class GLVertexBuffer;
	class GLIndexBuffer;
	class GLUniformBuffer;
	class GLTexture2D;
	class GLProgram;


	struct GLContextLimits
	{
		std::string ToString() const;

		int MaxTextureImageUnits;
		int MaxCombinedTextureImageUnits;
		int MaxElementIndex;
		int MaxDrawBuffers;
		int MaxUniformBufferBindings;
		int MaxUniformBlockSize;
	};

	// currently not enforced as a singleton class, but the general use case
	// involves using a single context which out-lives all other GL resources.
	class GLContext
	{
	public:
		GLContext();
		GLContext(const GLContext&) = delete;
		~GLContext();

		GLContext& operator=(const GLContext&) = delete;

		void Setup();
		void Cleanup();

		//void BindVAO(GLVertexArray* vao);
		void BindVBO(GLVertexBuffer& vbo);
		void BindIBO(GLIndexBuffer& ibo);
		void BindUBO(GLUniformBuffer& ubo);
		//void BindTextureUnit(int unit);
		//void BindTexture2D(GLTexture2D& texture);
		//void BindProgram(GLProgram& program);

		inline GLuint GetVersion() const { return mVersion; }
		inline const GLContextLimits& GetLimits() const { return sLimits; }
		
	private:
		void QueryLimits();

		static bool sHasGladInit;
		static GLContextLimits sLimits;

		GLuint mVersion;
		GLVertexArray* mBoundVAO = nullptr;
		UID mBoundVBO = 0;
		UID mBoundUBO = 0;
		UID mBoundProgram = 0;
		UID mBoundTexture2D = 0;
		int mBoundTextureUnit = 0;
	};

} // namespace LD