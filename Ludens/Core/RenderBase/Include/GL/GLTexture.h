#pragma once

#include <glad/glad.h>
#include "Core/OS/Include/UID.h"

namespace LD {

	class GLContext;

	struct GLTexture2DInfo
	{
		const void* Data = nullptr;
		GLenum InternalFormat;
		GLenum DataFormat;
		GLenum DataType = GL_UNSIGNED_BYTE;
		u16 Width;
		u16 Height;
	};

	class GLTexture2D
	{
	public:
		GLTexture2D();
		GLTexture2D(const GLTexture2D&) = delete;
		~GLTexture2D();

		GLTexture2D& operator=(const GLTexture2D&) = delete;

		void Setup(GLContext& context, const GLTexture2DInfo& info);
		void Cleanup();
		void Bind(int unit);

		inline UID GetHandle() const { return (UID)mHandle; }
		inline explicit operator UID() const { return (UID)mHandle; }
		inline explicit operator GLuint() const { return mTexture; }

	private:
		CUID<GLTexture2D> mHandle;
		GLContext* mContext = nullptr;
		GLuint mTexture;
	};

} // namespace LD