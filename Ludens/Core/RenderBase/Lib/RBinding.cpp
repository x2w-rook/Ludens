#include "Core/RenderBase/Include/RBinding.h"
#include "Core/RenderBase/Lib/RBindingGL.h"

namespace LD {

	RResult RBindingGroup::BindTexture(u32 bindingIdx, RTexture& textureH)
	{
		return mGroup->BindTexture(bindingIdx, textureH);
	}

	RResult RBindingGroup::BindUniformBuffer(u32 bindingIdx, RBuffer& bufferH)
	{
		return mGroup->BindUniformBuffer(bindingIdx, bufferH);
	}


} // namespace LD