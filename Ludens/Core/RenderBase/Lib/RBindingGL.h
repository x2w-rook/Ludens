#pragma once

#include "Core/OS/Include/UID.h"
#include "Core/RenderBase/Include/RBinding.h"
#include "Core/RenderBase/Lib/RBase.h"

namespace LD {

	struct RBindingGroupLayoutGL : RBindingGroupLayoutBase
	{
		RBindingGroupLayoutGL();
		RBindingGroupLayoutGL(const RBindingGroupLayoutGL&) = delete;
		~RBindingGroupLayoutGL();

		RBindingGroupLayoutGL& operator=(const RBindingGroupLayoutGL&) = delete;

		void Startup(RBindingGroupLayout& layoutH, const RBindingGroupLayoutInfo& info, RDeviceGL& device);
		void Cleanup(RBindingGroupLayout& layoutH);

		CUID<RBindingGroupLayoutGL> ID;
	};

	struct RBindingGroupGL : RBindingGroupBase
	{
		RBindingGroupGL();
		RBindingGroupGL(const RBindingGroupGL&) = delete;
		~RBindingGroupGL();

		RBindingGroupGL& operator=(const RBindingGroupGL&) = delete;

		void Startup(RBindingGroup& groupH, const RBindingGroupInfo& info, RDeviceGL& device);
		void Cleanup(RBindingGroup& groupH);

		virtual RResult BindTexture(u32 binding, RTexture& textureH) override;
		virtual RResult BindUniformBuffer(u32 binding, RBuffer& bufferH) override;
	};

} // namespace LD