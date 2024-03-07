#include "Core/RenderBase/Include/RFrameBuffer.h"
#include "Core/RenderBase/Include/RResult.h"
#include "Core/RenderBase/Lib/RBase.h"

namespace LD {
	
	RResult RFrameBuffer::GetInfo(RFrameBufferInfo* info)
	{
		*info = mFrameBuffer->Info;

		RResult result{};
		mFrameBuffer->Device->Callback(result);
		return result;
	}

	RResult RFrameBuffer::GetColorAttachment(int idx, RTexture* colorAttachment)
	{
		RResult result = mFrameBuffer->GetColorAttachment(idx, colorAttachment);
		mFrameBuffer->Device->Callback(result);
		return result;
	}

	RResult RFrameBuffer::GetDepthStencilAttachment(RTexture* depthStencilAttachment)
	{
		RResult result = mFrameBuffer->GetDepthStencilAttachment(depthStencilAttachment);
		mFrameBuffer->Device->Callback(result);
		return result;
	}

	RResult RFrameBuffer::Invalidate(const RFrameBufferInfo& info)
	{
		// NOTE: invalidation should not modify handle ID, since it might be cached somewhere by user code.
		UID oldID = (UID)mFrameBuffer;
		RResult result = mFrameBuffer->Invalidate(info);
		LD_DEBUG_ASSERT((UID)mFrameBuffer == oldID);

		mFrameBuffer->Device->Callback(result);
		return result;
	}

} // namespace LD

