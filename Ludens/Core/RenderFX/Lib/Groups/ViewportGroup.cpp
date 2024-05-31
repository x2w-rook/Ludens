#include "Core/DSA/Include/Array.h"
#include "Core/RenderFX/Include/Groups/ViewportGroup.h"

namespace LD {

ViewportGroup::~ViewportGroup()
{
    LD_DEBUG_ASSERT(!mDevice);
}

void ViewportGroup::Startup(RDevice device, RBindingGroupLayout viewportBGL)
{
    LD_DEBUG_ASSERT(device);
    mDevice = device;

    RBufferInfo bufferI;
    bufferI.Type = RBufferType::UniformBuffer;
    bufferI.MemoryUsage = RMemoryUsage::FrameDynamic;
    bufferI.Data = nullptr;
    bufferI.Size = sizeof(ViewportUBO);
    mDevice.CreateBuffer(mUBO, bufferI);

    RBindingGroupInfo bgI;
    bgI.Layout = viewportBGL;
    mDevice.CreateBindingGroup(mHandle, bgI);

    mHandle.BindUniformBuffer(0, mUBO);
}

void ViewportGroup::Cleanup()
{
    mDevice.DeleteBindingGroup(mHandle);
    mDevice.DeleteBuffer(mUBO);
    mDevice.ResetHandle();
}

RBindingGroupLayoutData ViewportGroup::GetLayoutData() const
{
    RBindingInfo binding0;
    binding0.Count = 1;
    binding0.Type = RBindingType::UniformBuffer;

    return { binding0 };
}

RBindingGroupLayout ViewportGroup::CreateLayout(RDevice device)
{
    LD_DEBUG_ASSERT((bool)device);

    RBindingGroupLayout viewportBGL;

    Array<RBindingInfo, 1> bindings{
        { RBindingType::UniformBuffer },
    };

    RBindingGroupLayoutInfo viewportBGLI;
    viewportBGLI.Bindings = bindings.GetView();
    device.CreateBindingGroupLayout(viewportBGL, viewportBGLI);

    return viewportBGL;
}

} // namespace LD