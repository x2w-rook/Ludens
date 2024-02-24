#include "Core/RenderBase/Include/GL/GLFrameBuffer.h"
#include "Core/RenderBase/Include/GL/GLContext.h"

namespace LD {

	GLFrameBuffer::GLFrameBuffer()
	{
	}

	GLFrameBuffer::~GLFrameBuffer()
	{
		LD_DEBUG_ASSERT(mHandle == 0);
	}

	void GLFrameBuffer::Setup(GLContext& context, const GLFrameBufferInfo& info)
	{
		mContext = &context;
		mHandle = CUID<GLFrameBuffer>::Get();
		mInfo = info;

		glCreateFramebuffers(1, &mFrameBuffer);
		mContext->BindFrameBuffer(*this);

		for (size_t i = 0; i < info.ColorAttachmentCount; i++)
		{
			GLTexture2D& attachment = *info.ColorAttachments[i];

			attachment.Bind(0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, (GLuint)attachment, 0);
		}

		if (info.DepthStencilAttachment)
		{
			GLTexture2D& attachment = *info.DepthStencilAttachment;

			attachment.Bind(0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, (GLuint)attachment, 0);
		}

		// TODO: MRT
		// GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		// glDrawBuffers(1, drawBuffers);

		LD_DEBUG_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

		mContext->UnbindFrameBuffer();
	}

	void GLFrameBuffer::Cleanup()
	{
		mHandle.Reset();
		mContext = nullptr;

		glDeleteFramebuffers(1, &mFrameBuffer);
	}

	void GLFrameBuffer::Bind()
	{
		mContext->BindFrameBuffer(*this);
	}

} // namespace LD