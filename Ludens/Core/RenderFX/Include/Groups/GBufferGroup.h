#pragma once

#include "Core/RenderBase/Include/RBinding.h"
#include "Core/RenderFX/Include/PrefabBindingGroup.h"

namespace LD
{

class GBuffer;

class GBufferGroup : public PrefabBindingGroup
{
public:
    /// @brief startup the viewport binding group
    /// @param device the owner device
    /// @param gbufferBGL a layout compatible with the GBuffer binding group, such as from CreateLayout()
    /// @param gbuffer the framebuffer instance to sample color attachments from
    void Startup(RDevice device, RBindingGroupLayout gbufferBGL, const GBuffer& gbuffer);

    /// cleanup the viewport binding group, the input BGL during Startup() is not deleted
    void Cleanup();

    virtual RBindingGroupLayoutData GetLayoutData() const override;

    virtual RBindingGroupLayout CreateLayout(RDevice device) override;

private:
    RDevice mDevice;
};

} // namespace LD