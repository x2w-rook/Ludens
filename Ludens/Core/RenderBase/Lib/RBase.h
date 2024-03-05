#pragma once

#include <string>
#include "Core/DSA/Include/Array.h"
#include "Core/RenderBase/Include/RTexture.h"
#include "Core/RenderBase/Include/RBuffer.h"
#include "Core/RenderBase/Include/RShader.h"
#include "Core/RenderBase/Include/RFrameBuffer.h"
#include "Core/RenderBase/Include/RPipeline.h"
#include "Core/RenderBase/Include/RBinding.h"


namespace LD {

	template <typename THandle, typename TBase = THandle::TBase>
	inline TBase& Unwrap(const THandle& handle)
	{
		LD_DEBUG_ASSERT((bool)handle);

		return *static_cast<TBase*>(handle);
	}

	template <typename TDerived, typename THandle, typename TBase = THandle::TBase>
	inline TDerived& Derive(const THandle& handle)
	{
		LD_DEBUG_ASSERT((bool)handle);

		TDerived* derived = dynamic_cast<TDerived*>((TBase*)handle);

		LD_DEBUG_ASSERT(derived != nullptr);
		return *derived;
	}

	struct RDeviceBase
	{
		RDeviceBase() = default;
		RDeviceBase(const RDeviceBase&) = delete;
		virtual ~RDeviceBase();

		RDeviceBase& operator=(const RDeviceBase&) = delete;

		void Setup(RDevice& deviceH, const RDeviceInfo& info);
		void Cleanup(RDevice& deviceH);

		virtual RResult CreateTexture(RTexture& texture, const RTextureInfo& info) = 0;
		virtual RResult DeleteTexture(RTexture& texture) = 0;

		virtual RResult CreateBuffer(RBuffer& buffer, const RBufferInfo& info) = 0;
		virtual RResult DeleteBuffer(RBuffer& buffer) = 0;

		virtual RResult CreateShader(RShader& shader, const RShaderInfo& info) = 0;
		virtual RResult DeleteShader(RShader& shader) = 0;

		virtual RResult CreateFrameBuffer(RFrameBuffer& frameBufferH, const RFrameBufferInfo& info) = 0;
		virtual RResult DeleteFrameBuffer(RFrameBuffer& frameBufferH) = 0;

		virtual RResult CreateBindingGroupLayout(RBindingGroupLayout& layout, const RBindingGroupLayoutInfo& info) = 0;
		virtual RResult DeleteBindingGroupLayout(RBindingGroupLayout& layout) = 0;

		virtual RResult CreateBindingGroup(RBindingGroup& group, const RBindingGroupInfo& info) = 0;
		virtual RResult DeleteBindingGroup(RBindingGroup& group) = 0;

		virtual RResult CreatePipeline(RPipeline& pipeline, const RPipelineInfo& info) = 0;
		virtual RResult DeletePipeline(RPipeline& pipeline) = 0;

		virtual RResult SetPipeline(RPipeline& pipeline) = 0;
		virtual RResult SetBindingGroup(RBindingGroup& group, u32 slot) = 0;
		virtual RResult SetVertexBuffer(RBuffer& buffer, u32 slot) = 0;
		virtual RResult SetIndexBuffer(RBuffer& buffer) = 0;
		virtual RResult SetFrameBuffer(RFrameBuffer* frameBuffer) = 0;


		virtual RResult DrawVertex(const RDrawVertexInfo& info) = 0;
		virtual RResult DrawIndexed(const RDrawIndexedInfo& info) = 0;

		CUID<RDeviceBase> ID;
		RResultCallback Callback;
		RPipeline BoundPipelineH;
	};

	struct RTextureBase
	{
		RTextureBase() = default;
		RTextureBase(const RTextureBase&) = delete;
		virtual ~RTextureBase();

		RTextureBase& operator=(const RTextureBase&) = delete;

		void Setup(RTexture& textureH, const RTextureInfo& info, RDeviceBase* device);
		void Cleanup(RTexture& textureH);

		CUID<RTextureBase> ID;
		RDeviceBase* Device = nullptr;
	};

	struct RBufferBase
	{
		RBufferBase() = default;
		RBufferBase(const RBufferBase&) = delete;
		virtual ~RBufferBase();

		RBufferBase& operator=(const RBufferBase&) = delete;

		void Setup(RBuffer& bufferH, const RBufferInfo& info, RDeviceBase* device);
		void Cleanup(RBuffer& bufferH);

		virtual RResult SetData(u32 offset, u32 size, const void* data) = 0;

		CUID<RBufferBase> ID;
		RDeviceBase* Device = nullptr;
		RBufferType Type;
	};

	struct RShaderBase
	{
		RShaderBase() = default;
		RShaderBase(const RShaderBase&) = delete;
		virtual ~RShaderBase();

		RShaderBase& operator=(const RShaderBase&) = delete;

		void Setup(RShader& shaderH, const RShaderInfo& info, RDeviceBase* device);
		void Cleanup(RShader& shaderH);

		CUID<RShaderBase> ID;
		RDeviceBase* Device = nullptr;
		RShaderType Type;
	};

	struct RFrameBufferBase
	{
		RFrameBufferBase() = default;
		RFrameBufferBase(const RFrameBufferBase&) = delete;
		virtual ~RFrameBufferBase();

		RFrameBufferBase& operator=(const RFrameBufferBase&) = delete;

		void Setup(RFrameBuffer& bufferH, const RFrameBufferInfo& info, RDeviceBase* device);
		void Cleanup(RFrameBuffer& bufferH);

		void SetupAttachments();
		void CleanupAttachments();

		virtual RResult Invalidate(const RFrameBufferInfo& info) = 0;

		RResult GetColorAttachment(int idx, RTexture* colorAttachment);
		RResult GetDepthStencilAttachment(RTexture* depthStencilAttachment);

		CUID<RFrameBufferBase> ID;
		RDeviceBase* Device = nullptr;
		RFrameBufferInfo Info;
		Array<RTexture, 8> ColorAttachments;
		RTexture DepthStencilAttachment;
	};

	struct RBindingGroupLayoutBase
	{
		RBindingGroupLayoutBase() = default;
		RBindingGroupLayoutBase(const RBindingGroupLayoutBase&) = delete;
		virtual ~RBindingGroupLayoutBase();

		RBindingGroupLayoutBase& operator=(const RBindingGroupLayoutBase&) = delete;

		void Setup(RBindingGroupLayout& layoutH, const RBindingGroupLayoutInfo& info, RDeviceBase* device);
		void Cleanup(RBindingGroupLayout& layoutH);

		inline size_t GetBindingCount() const
		{
			return Bindings.Size();
		}

		inline bool HasSameLayout(const RBindingGroupLayoutBase& other)
		{
			if (Bindings.Size() != other.Bindings.Size())
				return false;

			for (size_t i = 0; i < Bindings.Size(); i++)
				if (Bindings[i].Type != other.Bindings[i].Type)
					return false;

			return true;
		}

		CUID<RBindingGroupLayoutBase> ID;
		RDeviceBase* Device = nullptr;
		Vector<RBindingInfo> Bindings;
	};

	struct RBindingGroupBase
	{
		RBindingGroupBase() = default;
		RBindingGroupBase(const RBindingGroupBase&) = delete;
		virtual ~RBindingGroupBase();

		RBindingGroupBase& operator=(const RBindingGroupBase&) = delete;

		void Setup(RBindingGroup& groupH, const RBindingGroupInfo& info, RDeviceBase* device);
		void Cleanup(RBindingGroup& groupH);

		virtual RResult BindTexture(RTexture& textureH, u32 slot) = 0;
		virtual RResult BindUniformBuffer(RBuffer& bufferH, u32 slot) = 0;

		struct Binding
		{
			std::string Name;
			RBindingType Type;

			union
			{
				RTexture TextureH;
				RBuffer BufferH;
			};
		};

		CUID<RBindingGroupBase> ID;
		RDeviceBase* Device = nullptr;
		Vector<Binding> Bindings;
		RBindingGroupLayout GroupLayoutH;
	};

	struct RPipelineBase
	{
		RPipelineBase() = default;
		RPipelineBase(const RPipelineBase&) = delete;
		virtual ~RPipelineBase();

		RPipelineBase& operator=(const RPipelineBase&) = delete;

		void Setup(RPipeline& pipelineH, const RPipelineInfo& info, RDeviceBase* device);
		void Cleanup(RPipeline& pipelineH);

		CUID<RPipelineBase> ID;
		RDeviceBase* Device = nullptr;
		std::string Name;
		RVertexLayout VertexLayout;
		RShader VertexShaderH;
		RShader FragmentShaderH;
		Vector<RBindingGroupLayout> GroupLayoutsH;
	};



} // namespace LD