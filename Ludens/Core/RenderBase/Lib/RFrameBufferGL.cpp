#include <algorithm>
#include <algorithm>
#include <glad/glad.h>
#include "Core/Header/Include/Error.h"
#include "Core/RenderBase/Lib/RFrameBufferGL.h"
#include "Core/RenderBase/Lib/RDeviceGL.h"
#include "Core/RenderBase/Lib/RDeriveGL.h"


namespace LD {


	RFrameBufferGL::RFrameBufferGL()
	{
	}

	RFrameBufferGL::~RFrameBufferGL()
	{
	}

	void RFrameBufferGL::Setup(RFrameBuffer& frameBufferH, const RFrameBufferInfo& info, RDeviceGL& device)
	{
		RFrameBufferBase::Setup(frameBufferH, info, (RDeviceBase*)&device);

		SetupGLAttachments();
	}

	void RFrameBufferGL::Cleanup(RFrameBuffer& frameBufferH)
	{
		RFrameBufferBase::Cleanup(frameBufferH);

		CleanupGLAttachments();
	}

	void RFrameBufferGL::SetupGLAttachments()
	{
		RDeviceGL* deviceGL = dynamic_cast<RDeviceGL*>(Device);

		size_t colorAttachmentCount = Info.ColorAttachmentInfos.Size();
		Vector<GLTexture2D*> glColorAttachments{ colorAttachmentCount };
		for (size_t i = 0; i < colorAttachmentCount; i++)
		{
			RTextureGL& glTexture = Derive<RTextureGL>(ColorAttachments[i]);
			glColorAttachments[i] = &glTexture.Texture2D;
		}

		GLTexture2D* glDepthStencilAttachment = nullptr;
		if (DepthStencilAttachment)
		{
			RTextureGL& glTexture = Derive<RTextureGL>(DepthStencilAttachment);
			glDepthStencilAttachment = &glTexture.Texture2D;
		}

		GLFrameBufferInfo glInfo{};
		glInfo.Width = Info.Width;
		glInfo.Height = Info.Height;
		glInfo.ColorAttachmentCount = glColorAttachments.Size();
		glInfo.ColorAttachments = glColorAttachments.Data();
		glInfo.DepthStencilAttachment = glDepthStencilAttachment;
		FBO.Setup(deviceGL->Context, glInfo);
	}

	void RFrameBufferGL::CleanupGLAttachments()
	{
		FBO.Cleanup();
	}

	RResult RFrameBufferGL::Invalidate(const RFrameBufferInfo& info)
	{
		RDeviceGL* deviceGL = dynamic_cast<RDeviceGL*>(Device);

		// NOTE: Calling Cleanup and Setup with the new info is feasible, but doing so will regenerate a UID for RFrameBuffer handle.
		//       Here we are trying to preserve the original UID of the RFrameBuffer handle, only the OpenGL objects are recreated.

		CleanupAttachments();
		CleanupGLAttachments();

		Info = info;

		SetupAttachments();
		SetupGLAttachments();

		return {};
	}

} // namespace LD
