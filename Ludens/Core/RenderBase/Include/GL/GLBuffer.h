#pragma once

#include <cstdint>
#include <glad/glad.h>
#include "Core/OS/Include/UID.h"


namespace LD {

	class GLContext;

	struct GLVertexBufferInfo
	{
		GLenum Usage;
		u32 Size = 0;
		const void* Data = nullptr;
	};

	class GLVertexBuffer
	{
	public:
		GLVertexBuffer();
		GLVertexBuffer(const GLVertexBuffer&) = delete;
		~GLVertexBuffer();

		GLVertexBuffer& operator=(const GLVertexBuffer&) = delete;

		void Setup(GLContext& context, const GLVertexBufferInfo& info);
		void Cleanup();
		void Bind();
		void SetData(u32 offset, u32 size, const void* data);

		inline UID GetHandle() const { return (UID)mHandle; }
		inline explicit operator UID() const { return (UID)mHandle; }
		inline explicit operator GLuint() const { return mVBO; }

	private:
		CUID<GLVertexBuffer> mHandle;
		GLContext* mContext = nullptr;
		GLuint mVBO;
		u32 mSize = 0;
	};

	struct GLIndexBufferInfo
	{
		GLenum Usage;
		u32 Size = 0;
		const void* Data = nullptr;

		// GL_UNSIGNED_BYTE:   8 bit indices
		// GL_UNSIGNED_SHORT: 16 bit indices
		// GL_UNSIGNED_INT:   32 bit indices
		GLenum IndexSize = GL_UNSIGNED_SHORT;
	};

	class GLIndexBuffer
	{
	public:
		GLIndexBuffer();
		GLIndexBuffer(const GLIndexBuffer&) = delete;
		~GLIndexBuffer();

		GLIndexBuffer& operator=(const GLIndexBuffer&) = delete;

		void Setup(GLContext& context, const GLIndexBufferInfo& spec);
		void Cleanup();
		void Bind();
		
		inline GLenum GetIndexSize() const { return mIndexSize; }
		inline UID GetHandle() const { return (UID)mHandle; }
		inline explicit operator UID() const { return (UID)mHandle; }
		inline explicit operator GLuint() const { return mIBO; }

	private:
		CUID<GLIndexBuffer> mHandle;
		GLContext* mContext = nullptr;
		GLuint mIBO;
		GLenum mIndexSize;
	};

	struct GLUniformBufferInfo
	{
		GLenum Usage = GL_STATIC_DRAW;
		u32 Size;
	};

	class GLUniformBuffer
	{
	public:
		GLUniformBuffer();
		GLUniformBuffer(const GLUniformBuffer&) = delete;
		~GLUniformBuffer();

		GLUniformBuffer& operator=(const GLUniformBuffer&) = delete;

		void Setup(GLContext& context, const GLUniformBufferInfo& info);
		void Cleanup();
		void Bind();
		void BindBase(int binding);
		void SetData(u32 offset, u32 size, const void* data);

		inline UID GetHandle() const { return (UID)mHandle; }
		inline explicit operator UID() const { return (UID)mHandle; }
		inline explicit operator GLuint() const { return mUBO; }

	private:
		CUID<GLUniformBuffer> mHandle;
		GLContext* mContext = nullptr;
		GLuint mUBO;
		u32 mSize = 0;
	};

} // namespace LD