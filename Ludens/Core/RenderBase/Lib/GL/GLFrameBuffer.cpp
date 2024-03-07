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

		size_t colorAttachmentCount = info.ColorAttachmentCount;

		for (size_t i = 0; i < colorAttachmentCount; i++)
		{
			GLTexture2D& attachment = *info.ColorAttachments[i];

			attachment.Bind(0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, (GLuint)attachment, 0);
		}

		if (info.DepthStencilAttachment)
		{
			GLTexture2D& attachment = *info.DepthStencilAttachment;

			// TODO: currently assumes GL_DEPTH_STENCIL_ATTACHMENT, check if attachment is depth only or stencil only

			attachment.Bind(0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, (GLuint)attachment, 0);
		}

		Vector<GLenum> drawBuffers(colorAttachmentCount);
		for (size_t i = 0; i < colorAttachmentCount; i++)
		{
			drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
		}
		glDrawBuffers(colorAttachmentCount, drawBuffers.Data());

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