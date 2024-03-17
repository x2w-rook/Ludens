#pragma once

#include <glad/glad.h>
#include "Core/Header/Include/Types.h"
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

	struct GLTexture2DArrayInfo
	{
		const void* Data = nullptr;
		GLenum DataFormat;
		GLenum DataType;
		GLenum InternalFormat;
		u32 Width;
		u32 Height;
		u32 Layers;
	};

	class GLTexture2DArray
	{
	public:
		GLTexture2DArray();
		GLTexture2DArray(const GLTexture2DArray&) = delete;
		~GLTexture2DArray();

		GLTexture2DArray& operator=(const GLTexture2DArray&) = delete;

		void Setup(GLContext& context, const GLTexture2DArrayInfo& info);
		void Cleanup();
		void Bind(int unit);

		inline UID GetHandle() const { return (UID)mHandle; }
		inline explicit operator UID() const { return (UID)mHandle; }
		inline explicit operator GLuint() const { return mTexture; }

	private:
		CUID<GLTexture2DArray> mHandle;
		GLContext* mContext = nullptr;
		GLuint mTexture;
		u32 mWidth;
		u32 mHeight;
		u32 mLayers;
	};

} // namespace LD