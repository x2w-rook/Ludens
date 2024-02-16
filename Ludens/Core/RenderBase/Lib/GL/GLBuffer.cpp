#include "Core/RenderBase/Include/GL/GLBuffer.h"
#include "Core/RenderBase/Include/GL/GLContext.h"
#include "Core/Header/Include/Error.h"

namespace LD {

	GLVertexBuffer::GLVertexBuffer()
		: mContext(nullptr)
	{
	}

	GLVertexBuffer::~GLVertexBuffer()
	{
		LD_DEBUG_ASSERT(mHandle == 0);
	}

	void GLVertexBuffer::Setup(GLContext& context, const GLVertexBufferInfo& info)
	{
		mHandle = CUID<GLVertexBuffer>::Get();
		mContext = &context;
		mSize = info.Size;

		glCreateBuffers(1, &mVBO);
		if (info.Size > 0)
		{
			context.BindVBO(*this);
			glBufferData(GL_ARRAY_BUFFER, info.Size, info.Data, info.Usage);
		}
	}

	void GLVertexBuffer::Cleanup()
	{
		glDeleteBuffers(1, &mVBO);

		mHandle.Reset();
		mContext = nullptr;
	}

	GLIndexBuffer::GLIndexBuffer()
		: mContext(nullptr)
	{
	}

	GLIndexBuffer::~GLIndexBuffer()
	{
		LD_DEBUG_ASSERT(mHandle == 0);
	}

	void GLVertexBuffer::Bind()
	{
		LD_DEBUG_ASSERT(mContext != nullptr);

		mContext->BindVBO(*this);
	}

	void GLVertexBuffer::SetData(u32 offset, u32 size, const void* data)
	{
		LD_DEBUG_ASSERT(offset + size <= mSize);

		Bind();
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	}

	void GLIndexBuffer::Setup(GLContext& context, const GLIndexBufferInfo& info)
	{
		mHandle = CUID<GLIndexBuffer>::Get();
		mIndexSize = info.IndexSize;
		mContext = &context;

		glCreateBuffers(1, &mIBO);
		if (info.Size > 0)
		{
			context.BindIBO(*this); // NOTE: this requires a VAO being bound first
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, info.Size, info.Data, info.Usage);
		}
	}

	void GLIndexBuffer::Cleanup()
	{
		glDeleteBuffers(1, &mIBO);

		mHandle.Reset();
		mContext = nullptr;
	}

	void GLIndexBuffer::Bind()
	{
		LD_DEBUG_ASSERT(mContext != nullptr);

		mContext->BindIBO(*this);
	}

	GLUniformBuffer::~GLUniformBuffer()
	{
		LD_DEBUG_ASSERT(mHandle == 0);
	}

	void GLUniformBuffer::Setup(GLContext& context, const GLUniformBufferInfo& info)
	{
		mHandle = CUID<GLUniformBuffer>::Get();
		mContext = &context;
		mSize = info.Size;

		glCreateBuffers(1, &mUBO);
		Bind();
		glBufferData(GL_UNIFORM_BUFFER, mSize, nullptr, info.Usage);
	}

	void GLUniformBuffer::Cleanup()
	{
		glDeleteBuffers(1, &mUBO);

		mHandle.Reset();
		mContext = nullptr;
		mSize = 0;
	}

	void GLUniformBuffer::Bind()
	{
		LD_DEBUG_ASSERT(mContext != nullptr);

		mContext->BindUBO(*this);
	}

	void GLUniformBuffer::BindBase(int binding)
	{
		Bind();
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, mUBO);
	}

	void GLUniformBuffer::SetData(u32 offset, u32 size, const void* data)
	{
		LD_DEBUG_ASSERT(offset + size <= mSize);

		Bind();
		glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	}

} // namespace LD