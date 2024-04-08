#pragma once

#include "Core/Header/Include/Types.h"
#include "Core/Math/Include/Vec4.h"
#include "Core/OS/Include/UID.h"
#include "Core/DSA/Include/View.h"
#include "Core/DSA/Include/Optional.h"
#include "Core/RenderBase/Include/RTexture.h"
#include "Core/RenderBase/Include/RResult.h"
#include "Core/RenderBase/Include/RPass.h"

namespace LD {

	struct RFrameBufferInfo
	{
		u32 Width = 0;
		u32 Height = 0;
		View<RTexture> ColorAttachments;
		Optional<RTexture> DepthStencilAttachment;
		RPass RenderPass;
	};

	struct RFrameBufferBase;
	struct RFrameBufferGL;

	// frame buffer handle and interface
	class RFrameBuffer
	{
		friend struct RFrameBufferBase;
		friend struct RFrameBufferGL;
	public:
		using TBase = RFrameBufferBase;

		RResult GetColorAttachment(int idx, RTexture* colorAttachment);
		RResult GetDepthStencilAttachment(RTexture* depthStencilAttachment);

		RResult Invalidate(const RFrameBufferInfo& info);

		inline operator bool() const { return mID != 0 && mFrameBuffer != nullptr; }
		inline operator RFrameBufferBase*() const { return mFrameBuffer; }
		inline UID GetID() const { return mID; }

		inline bool operator==(const RFrameBuffer& other) const { return mID == other.mID; }
		inline bool operator!=(const RFrameBuffer& other) const { return mID != other.mID; }

	private:
		UID mID = 0;
		RFrameBufferBase* mFrameBuffer = nullptr;
	};

	using RClearColorValue = Vec4;
	
	struct RClearDepthStencilValue
	{
		f32 Depth = 1.0f;
		u8 Stencil = 0;
	};

	struct RClearValue
	{
		Optional<RClearColorValue> Color;
		Optional<RClearDepthStencilValue> DepthStencil;
	};

	struct RPassBeginInfo
	{
		RPass RenderPass;
		RFrameBuffer FrameBuffer;
		View<RClearValue> ClearValues;
	};

} // namespace LD
