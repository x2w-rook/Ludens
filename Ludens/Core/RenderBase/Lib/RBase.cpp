#include <cstring>
#include "Core/RenderBase/Lib/RBase.h"
#include "Core/Header/Include/Error.h"

namespace LD {

	///
	/// Device Base
	///
	
	RDeviceBase::~RDeviceBase()
	{
		LD_DEBUG_ASSERT(ID == 0);
	}

	void RDeviceBase::Startup(RDevice& deviceH, const RDeviceInfo& info)
	{
		ID = CUID<RDeviceBase>::Get();
		Callback = info.Callback ? info.Callback : [](const RResult&) {};

		// connect
		deviceH.mID = ID;
		deviceH.mDevice = this;
	}

	void RDeviceBase::Cleanup(RDevice& deviceH)
	{
		ID.Reset();

		// disconnect
		deviceH.mID = 0;
		deviceH.mDevice = nullptr;
	}

	///
	/// Texture Base
	///

	RTextureBase::~RTextureBase()
	{
		LD_DEBUG_ASSERT(ID == 0);
	}

	void RTextureBase::Startup(RTexture& textureH, RDeviceBase* device)
	{
		ID = CUID<RTextureBase>::Get();
		Device = device;

		// connect
		textureH.mID = ID;
		textureH.mTexture = this;
	}

	void RTextureBase::Startup(RTexture& textureH, const RTextureInfo& info, RDeviceBase* device)
	{
		(void)info;

		Startup(textureH, device);
	}

	void RTextureBase::Cleanup(RTexture& textureH)
	{
		ID.Reset();
		Device = nullptr;

		// disconnect
		textureH.mID = 0;
		textureH.mTexture = nullptr;
	}

	///
	/// Buffer Base
	///

	RBufferBase::~RBufferBase()
	{
		LD_DEBUG_ASSERT(ID == 0);
	}

	void RBufferBase::Startup(RBuffer& bufferH, const RBufferInfo& info, RDeviceBase* device)
	{
		ID = CUID<RBufferBase>::Get();
		Device = device;
		Type = info.Type;

		// connect
		bufferH.mID = ID;
		bufferH.mBuffer = this;
	}

	void RBufferBase::Cleanup(RBuffer& bufferH)
	{
		ID.Reset();
		Device = nullptr;

		// disconnect
		bufferH.mID = 0;
		bufferH.mBuffer = nullptr;
	}

	///
	/// Shader Base
	///

	RShaderBase::~RShaderBase()
	{
		LD_DEBUG_ASSERT(ID == 0);
	}

	void RShaderBase::Startup(RShader& shaderH, const RShaderInfo& info, RDeviceBase* device)
	{
		ID = CUID<RShaderBase>::Get();
		Device = device;
		SourceType = info.SourceType;
		Type = info.Type;

		// connect
		shaderH.mID = ID;
		shaderH.mShader = this;
	}

	void RShaderBase::Cleanup(RShader& shaderH)
	{
		ID.Reset();
		Device = nullptr;

		// disconnect
		shaderH.mID = 0;
		shaderH.mShader = nullptr;
	}

	///
	/// Binding Group Layout Base
	///

	RBindingGroupLayoutBase::~RBindingGroupLayoutBase()
	{
		LD_DEBUG_ASSERT(ID == 0);
	}

	void RBindingGroupLayoutBase::Startup(RBindingGroupLayout& layoutH, const RBindingGroupLayoutInfo& info, RDeviceBase* device)
	{
		ID = CUID<RBindingGroupLayoutBase>::Get();
		Device = device;

		// copy binding information
		size_t bindingCount = info.Bindings.Size();
		Bindings.Resize(bindingCount);
		for (size_t i = 0; i < bindingCount; i++)
		{
			Bindings[i] = info.Bindings[i];
		}

		// connect
		layoutH.mID = ID;
		layoutH.mGroupLayout = this;
	}

	void RBindingGroupLayoutBase::Cleanup(RBindingGroupLayout& layoutH)
	{
		ID.Reset();
		Device = nullptr;

		// disconnect
		layoutH.mID = 0;
		layoutH.mGroupLayout = nullptr;
	}

	///
	/// Binding Group Base
	///

	RBindingGroupBase::~RBindingGroupBase()
	{
		LD_DEBUG_ASSERT(ID == 0);
	}

	void RBindingGroupBase::Startup(RBindingGroup& groupH, const RBindingGroupInfo& info, RDeviceBase* device)
	{
		ID = CUID<RBindingGroupBase>::Get();
		Device = device;

		GroupLayoutH = info.Layout;
		RBindingGroupLayoutBase& layout = *(RBindingGroupLayoutBase*)GroupLayoutH;

		size_t bindingCount = layout.Bindings.Size();
		Bindings.Resize(bindingCount);

		for (size_t i = 0; i < bindingCount; i++)
		{
			Bindings[i].Type = layout.Bindings[i].Type;

			if (Bindings[i].Type == RBindingType::Texture)
				Bindings[i].TextureH.Resize(layout.Bindings[i].Count);
		}

		// connect
		groupH.mID = ID;
		groupH.mGroup = this;
	}

	void RBindingGroupBase::Cleanup(RBindingGroup& groupH)
	{
		ID.Reset();
		Device = nullptr;

		// disconnect
		groupH.mID = 0;
		groupH.mGroup = nullptr;
	}

	///
	/// Render Pass Base
	///

	RPassBase::~RPassBase()
	{
		LD_DEBUG_ASSERT(ID == 0);
	}

	void RPassBase::Startup(RPass& passH, const RPassInfo& info, RDeviceBase* device)
	{
		ID = CUID<RPassBase>::Get();
		Device = device;

		Attachments.Resize(info.Attachments.Size());
		std::copy(info.Attachments.Begin(), info.Attachments.End(), Attachments.Begin());

		// connect
		passH.mID = ID;
		passH.mPass = this;
	}

	void RPassBase::Cleanup(RPass& passH)
	{
		ID.Reset();
		Device = nullptr;

		// disconnect
		passH.mID = 0;
		passH.mPass = nullptr;
	}

	///
	/// Frame Buffer Base
	///
	
	RFrameBufferBase::~RFrameBufferBase()
	{
		LD_DEBUG_ASSERT(ID == 0);
	}

	void RFrameBufferBase::ReadInfo(const RFrameBufferInfo& info)
	{
		Width = info.Width;
		Height = info.Height;

		ColorAttachments.Resize(info.ColorAttachments.Size());
		std::copy(info.ColorAttachments.Begin(), info.ColorAttachments.End(), ColorAttachments.Begin());

		DepthStencilAttachment = info.DepthStencilAttachment;
	}

	void RFrameBufferBase::Startup(RFrameBuffer& frameBufferH, const RFrameBufferInfo& info, RDeviceBase* device)
	{
		ID = CUID<RFrameBufferBase>::Get();

		Device = device;
		ReadInfo(info);

		LD_DEBUG_ASSERT(Width > 0 && Height > 0);

		// connect
		frameBufferH.mID = ID;
		frameBufferH.mFrameBuffer = this;
	}

	void RFrameBufferBase::Cleanup(RFrameBuffer& frameBufferH)
	{
		ID.Reset();
		Device = nullptr;

		// disconnect
		frameBufferH.mID = 0;
		frameBufferH.mFrameBuffer = nullptr;
	}

	RResult RFrameBufferBase::GetColorAttachment(int idx, RTexture* colorAttachment)
	{
		RResult result{};

		if (idx < 0 || idx >= ColorAttachments.Size())
		{
			result.Type = RResultType::InvalidIndex;
			return result;
		}

		*colorAttachment = ColorAttachments[idx];

		return result;
	}

	RResult RFrameBufferBase::GetDepthStencilAttachment(RTexture* depthStencilAttachment)
	{
		// TODO: RResult error code
		LD_DEBUG_ASSERT(DepthStencilAttachment.HasValue());

		*depthStencilAttachment = DepthStencilAttachment.Value();

		return {};
	}

	///
	/// Pipeline Base
	///

	RPipelineBase::~RPipelineBase()
	{
		LD_DEBUG_ASSERT(ID == 0);
	}

	void RPipelineBase::Startup(RPipeline& pipelineH, const RPipelineInfo& info, RDeviceBase* device)
	{
		ID = CUID<RPipelineBase>::Get();
		Device = device;

		if (info.Name)
			Name = { info.Name, strlen(info.Name) };

		VertexLayout = info.VertexLayout;
		VertexShaderH = info.VertexShader;
		FragmentShaderH = info.FragmentShader;
		DepthTestEnabled = info.DepthStencilState.DepthTestEnabled;
		DepthWriteEnabled = info.DepthStencilState.DepthWriteEnabled;
		CullMode = info.RasterizationState.CullMode;
		PolygonMode = info.RasterizationState.PolygonMode;

		const RPipelineLayout& pipelineLayout = info.PipelineLayout;

		GroupLayoutsH.Resize(pipelineLayout.GroupLayouts.Size());
		for (size_t i = 0; i < GroupLayoutsH.Size(); i++)
		{
			GroupLayoutsH[i] = pipelineLayout.GroupLayouts[i];
		}

		// connect
		pipelineH.mID = ID;
		pipelineH.mPipeline = this;
	}

	void RPipelineBase::Cleanup(RPipeline& pipelineH)
	{
		ID.Reset();
		Device = nullptr;

		GroupLayoutsH.Clear();

		// disconnect
		pipelineH.mID = 0;
		pipelineH.mPipeline = nullptr;
	}

} // namespace LD