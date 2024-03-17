#pragma once

#include "Core/Header/Include/Types.h"
#include "Core/OS/Include/UID.h"
#include "Core/DSA/Include/Optional.h"
#include "Core/RenderBase/Include/RResult.h"

namespace LD {


	class RDevice;
	class RDeviceBase;
	class RTexture;
	class RTextureInfo;
	class RBuffer;
	class RBufferInfo;
	class RShader;
	class RShaderInfo;
	class RFrameBuffer;
	class RFrameBufferInfo;
	class RBindingGroup;
	class RBindingGroupInfo;
	class RBindingGroupLayout;
	class RBindingGroupLayoutInfo;
	class RPipeline;
	class RPipelineInfo;

	using RResultCallback = void (*)(const RResult&);

	enum class RBackend
	{
		OpenGL = 0,
		Vulkan,     // not implemented yet, but shaders are written in Vulkan GLSL
	};

	struct RDeviceInfo
	{
		RBackend Backend = RBackend::OpenGL;
		RResultCallback Callback = nullptr;
	};

	struct RDrawVertexInfo
	{
		u32 VertexCount = 0;
		u32 VertexStart = 0;
		u32 InstanceCount = 1;
		u32 InstanceStart = 0;
	};

	enum class RIndexType
	{
		u16 = 0,
		u32
	};

	struct RDrawIndexedInfo
	{
		RIndexType IndexType;
		u32 IndexCount = 0;
		u32 IndexStart = 0;
		u32 InstanceCount = 1;
		u32 InstanceStart = 0;
	};

	RResult CreateRenderDevice(RDevice& device, const RDeviceInfo& info);
	RResult DeleteRenderDevice(RDevice& device);

	struct RDrawStats
	{
		u32 DrawVertexCalls;
		u32 DrawIndexedCalls;
		u32 TotalVertices;

		inline u32 DrawCalls() const
		{
			return DrawVertexCalls + DrawIndexedCalls;
		}
	};

	// render device handle and interface
	class RDevice
	{
		friend struct RDeviceBase;
		friend struct RDeviceGL;
	public:
		using TBase = RDeviceBase;

		RResult CreateTexture(RTexture& texture, const RTextureInfo& info);
		RResult DeleteTexture(RTexture& texture);

		RResult CreateBuffer(RBuffer& buffer, const RBufferInfo& info);
		RResult DeleteBuffer(RBuffer& buffer);

		RResult CreateShader(RShader& shader, const RShaderInfo& info);
		RResult DeleteShader(RShader& shader);

		RResult CreateFrameBuffer(RFrameBuffer& frameBufferH, const RFrameBufferInfo& info);
		RResult DeleteFrameBuffer(RFrameBuffer& frameBufferH);

		RResult CreateBindingGroupLayout(RBindingGroupLayout& layout, const RBindingGroupLayoutInfo& info);
		RResult DeleteBindingGroupLayout(RBindingGroupLayout& layout);
		
		RResult CreateBindingGroup(RBindingGroup& group, const RBindingGroupInfo& info);
		RResult DeleteBindingGroup(RBindingGroup& group);

		RResult CreatePipeline(RPipeline& pipeline, const RPipelineInfo& info);
		RResult DeletePipeline(RPipeline& pipeline);

		RResult SetPipeline(RPipeline& pipeline);
		RResult SetBindingGroup(u32 slot, RBindingGroup& group);
		RResult SetVertexBuffer(u32 slot, RBuffer& buffer);
		RResult SetIndexBuffer(RBuffer& buffer);

		// TODO: Temporary solution before render pass and swap chain API, currently we are using OpenGL-like binding API.
		//       Calling with nullptr will bind the 'default' frame buffer of the swap chain.
		RResult SetFrameBuffer(RFrameBuffer* frameBuffer);
		
		// TODO: Multiple drawstats overlapping on one device.
		RResult BeginDrawStats(RDrawStats* stats);
		RResult EndDrawStats();

		RResult DrawVertex(const RDrawVertexInfo& info);
		RResult DrawIndexed(const RDrawIndexedInfo& info);

		inline operator bool() const { return mID != 0 && mDevice != nullptr; }
		inline RBackend GetBackend() const { return mBackend; }
		inline UID GetID() const { return mID; }
		inline void Reset() { mID = 0; mDevice = nullptr; }

		inline bool operator==(const RDevice& other) const { return mID == other.mID; }
		inline bool operator!=(const RDevice& other) const { return mID != other.mID; }

	private:
		UID mID = 0;
		RDeviceBase* mDevice = nullptr;
		RBackend mBackend;
	};

} // namespace LD