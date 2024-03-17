#include <iostream>
#include "Core/Header/Include/Error.h"
#include "Core/RenderBase/Lib/RBindingGL.h"
#include "Core/RenderBase/Lib/RTextureGL.h"
#include "Core/RenderBase/Lib/RDeviceGL.h"

namespace LD {

	RBindingGroupLayoutGL::RBindingGroupLayoutGL()
	{
	}

	RBindingGroupLayoutGL::~RBindingGroupLayoutGL()
	{
		LD_DEBUG_ASSERT(ID == 0);
	}

	void RBindingGroupLayoutGL::Setup(RBindingGroupLayout& layoutH, const RBindingGroupLayoutInfo& info, RDeviceGL& device)
	{
		RBindingGroupLayoutBase::Setup(layoutH, info, &device);
	}

	void RBindingGroupLayoutGL::Cleanup(RBindingGroupLayout& layoutH)
	{
		RBindingGroupLayoutBase::Cleanup(layoutH);
	}

	RBindingGroupGL::RBindingGroupGL()
	{
	}

	RBindingGroupGL::~RBindingGroupGL()
	{
		LD_DEBUG_ASSERT(ID == 0);
	}

	void RBindingGroupGL::Setup(RBindingGroup& groupH, const RBindingGroupInfo& info, RDeviceGL& device)
	{
		RBindingGroupBase::Setup(groupH, info, &device);

		groupH.mBackend = RBackend::OpenGL;
	}

	void RBindingGroupGL::Cleanup(RBindingGroup& groupH)
	{
		RBindingGroupBase::Cleanup(groupH);
	}

	RResult RBindingGroupGL::BindTexture(u32 binding, RTexture& textureH)
	{
		RBindingGroupLayoutGL& layout = Derive<RBindingGroupLayoutGL>(GroupLayoutH);

		LD_DEBUG_ASSERT(0 <= binding && binding < layout.Bindings.Size());
		LD_DEBUG_ASSERT(layout.Bindings.Size() == Bindings.Size());
		LD_DEBUG_ASSERT(layout.Bindings[binding].Type == RBindingType::Texture);

		// this texture will be visible to shader after RDeviceGL::SetBindingGroup
		Bindings[binding].TextureH = textureH;

		return {};
	}

	RResult RBindingGroupGL::BindUniformBuffer(u32 binding, RBuffer& bufferH)
	{
		RBindingGroupLayoutGL& layout = Derive<RBindingGroupLayoutGL>(GroupLayoutH);

		LD_DEBUG_ASSERT(0 <= binding && binding < layout.Bindings.Size());
		LD_DEBUG_ASSERT(layout.Bindings.Size() == Bindings.Size());
		LD_DEBUG_ASSERT(layout.Bindings[binding].Type == RBindingType::UniformBuffer);

		// this uniform buffer will be visible to shader after RDeviceGL::SetBindingGroup
		Bindings[binding].BufferH = bufferH;

		return {};
	}

} // namespace LD