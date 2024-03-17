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

	void RDeviceBase::Setup(RDevice& deviceH, const RDeviceInfo& info)
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

	void RTextureBase::Setup(RTexture& textureH, const RTextureInfo& info, RDeviceBase* device)
	{
		ID = CUID<RTextureBase>::Get();
		Device = device;

		// connect
		textureH.mID = ID;
		textureH.mTexture = this;
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

	void RBufferBase::Setup(RBuffer& bufferH, const RBufferInfo& info, RDeviceBase* device)
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

	void RShaderBase::Setup(RShader& shaderH, const RShaderInfo& info, RDeviceBase* device)
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

		ColorAttachmentCount = info.ColorAttachmentInfos.Size();

		for (size_t i = 0; i < ColorAttachmentCount; i++)
		{
			ColorAttachmentInfos[i] = info.ColorAttachmentInfos[i];
		}

		DepthStencilAttachmentInfo = info.DepthStencilAttachmentInfo;
	}

	void RFrameBufferBase::Setup(RFrameBuffer& frameBufferH, const RFrameBufferInfo& info, RDeviceBase* device)
	{
		ID = CUID<RFrameBufferBase>::Get();

		Device = device;
		ReadInfo(info);

		LD_DEBUG_ASSERT(Width > 0 && Height > 0);
		SetupAttachments();

		// connect
		frameBufferH.mID = ID;
		frameBufferH.mFrameBuffer = this;
	}

	void RFrameBufferBase::Cleanup(RFrameBuffer& frameBufferH)
	{
		CleanupAttachments();

		ID.Reset();
		Device = nullptr;

		// disconnect
		frameBufferH.mID = 0;
		frameBufferH.mFrameBuffer = nullptr;
	}

	void RFrameBufferBase::SetupAttachments()
	{
		LD_DEBUG_ASSERT(ColorAttachmentCount < ColorAttachments.Size());

		for (size_t i = 0; i < ColorAttachmentCount; i++)
		{
			const RAttachmentInfo& attachmentInfo = ColorAttachmentInfos[i];

			RTextureInfo textureInfo{};
			textureInfo.Data = nullptr;
			textureInfo.Type = RTextureType::Texture2D;
			textureInfo.Format = attachmentInfo.Format;
			textureInfo.Width = Width;
			textureInfo.Height = Height;
			Device->CreateTexture(ColorAttachments[i], textureInfo);
		}

		if (DepthStencilAttachmentInfo)
		{
			RAttachmentInfo& attachmentInfo = *DepthStencilAttachmentInfo;
			LD_DEBUG_ASSERT(attachmentInfo.Format == RTextureFormat::D24S8);

			RTextureInfo textureInfo{};
			textureInfo.Data = nullptr;
			textureInfo.Type = RTextureType::Texture2D;
			textureInfo.Format = attachmentInfo.Format;
			textureInfo.Width = Width;
			textureInfo.Height = Height;
			Device->CreateTexture(DepthStencilAttachment, textureInfo);
		}
	}

	void RFrameBufferBase::CleanupAttachments()
	{
		if (DepthStencilAttachmentInfo)
		{
			Device->DeleteTexture(DepthStencilAttachment);
		}

		for (size_t i = 0; i < ColorAttachmentCount; i++)
		{
			Device->DeleteTexture(ColorAttachments[i]);
		}
	}

	RResult RFrameBufferBase::GetColorAttachment(int idx, RTexture* colorAttachment)
	{
		RResult result{};

		if (idx < 0 || idx >= ColorAttachmentCount)
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
		LD_DEBUG_ASSERT(DepthStencilAttachmentInfo);

		*depthStencilAttachment = DepthStencilAttachment;

		return {};
	}

	///
	/// Binding Group Layout Base
	///

	RBindingGroupLayoutBase::~RBindingGroupLayoutBase()
	{
		LD_DEBUG_ASSERT(ID == 0);
	}

	void RBindingGroupLayoutBase::Setup(RBindingGroupLayout& layoutH, const RBindingGroupLayoutInfo& info, RDeviceBase* device)
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

	void RBindingGroupBase::Setup(RBindingGroup& groupH, const RBindingGroupInfo& info, RDeviceBase* device)
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
	/// Pipeline Base
	///

	RPipelineBase::~RPipelineBase()
	{
		LD_DEBUG_ASSERT(ID == 0);
	}

	void RPipelineBase::Setup(RPipeline& pipelineH, const RPipelineInfo& info, RDeviceBase* device)
	{
		ID = CUID<RPipelineBase>::Get();
		Device = device;

		if (info.Name)
			Name = { info.Name, strlen(info.Name) };

		VertexLayout = info.VertexLayout;
		VertexShaderH = info.VertexShader;
		FragmentShaderH = info.FragmentShader;

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