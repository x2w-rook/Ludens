#include "Core/DSA/Include/Array.h"
#include "Core/RenderFX/Include/Groups/GBufferGroup.h"
#include "Core/RenderFX/Include/FrameBuffers/GBuffer.h"

namespace LD
{

void GBufferGroup::Startup(RDevice device, RBindingGroupLayout gbufferBGL, const GBuffer& gbuffer)
{
    LD_DEBUG_ASSERT(device && gbufferBGL);
    LD_DEBUG_ASSERT(gbuffer);

    mDevice = device;
    
    RBindingGroupInfo groupI;
    groupI.Layout = gbufferBGL;
    mDevice.CreateBindingGroup(mHandle, groupI);

    // establish bindings with the GBuffer instance
    mHandle.BindTexture(0, gbuffer.GetPosition());
    mHandle.BindTexture(1, gbuffer.GetNormals());
    mHandle.BindTexture(2, gbuffer.GetAlbedo());
}

void GBufferGroup::Cleanup()
{
    mDevice.DeleteBindingGroup(mHandle);
    mDevice.ResetHandle();
}

RBindingGroupLayoutData GBufferGroup::GetLayoutData() const
{
    RBindingInfo binding;
    binding.Count = 1;
    binding.Type = RBindingType::Texture;

    return { binding, binding, binding };
}

RBindingGroupLayout GBufferGroup::CreateLayout(RDevice device)
{
    RBindingGroupLayout gbufferBGL;

    Array<RBindingInfo, 3> bindings{
        { RBindingType::Texture }, // position
        { RBindingType::Texture }, // normal
        { RBindingType::Texture }, // albedo
    };

    RBindingGroupLayoutInfo gbufferBGLI;
    gbufferBGLI.Bindings = bindings.GetView();
    device.CreateBindingGroupLayout(gbufferBGL, gbufferBGLI);

    return gbufferBGL;
}

} // namespace LD