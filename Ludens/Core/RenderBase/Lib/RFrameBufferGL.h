#pragma once

#include <glad/glad.h>
#include "Core/RenderBase/Include/GL/GLFrameBuffer.h"
#include "Core/RenderBase/Lib/RBase.h"

namespace LD {

	struct RDeviceGL;

	struct RFrameBufferGL : RFrameBufferBase
	{
		RFrameBufferGL();
		RFrameBufferGL(const RFrameBufferGL&) = delete;
		~RFrameBufferGL();

		RFrameBufferGL& operator=(const RFrameBufferGL&) = delete;

		void Setup(RFrameBuffer& frameBufferH, const RFrameBufferInfo& info, RDeviceGL& device);
		void Cleanup(RFrameBuffer& frameBufferH);

		void SetupGLAttachments();
		void CleanupGLAttachments();

		virtual RResult Invalidate(const RFrameBufferInfo& info) override;

		GLFrameBuffer FBO;
	};

} // namespace LD