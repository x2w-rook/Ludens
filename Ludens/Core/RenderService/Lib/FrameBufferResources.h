#pragma once

#include <Core/DSA/Include/Vector.h>
#include "Core/RenderService/Lib/RenderResources.h"
#include "Core/RenderFX/Include/FrameBuffers/GBuffer.h"
#include "Core/RenderFX/Include/FrameBuffers/SSAOBuffer.h"

namespace LD
{

class RenderPassResources;

class FrameBufferResources : public RenderResources
{
public:
    void Startup(RDevice device, RenderPassResources* passRes);
    void Cleanup();

    void CreateGBuffer(GBuffer& gbuffer, int width, int height);
    void CreateSSAOBuffer(SSAOBuffer& buffer, int width, int height, RPass pass);

private:
    RenderPassResources* mPassRes;
};

} // namespace LD