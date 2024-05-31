#pragma once

#include "Core/RenderFX/Include/Passes/GBufferPass.h"
#include "Core/RenderService/Lib/RenderResources.h"
#include "Core/RenderService/Lib/RenderPassResources.h"

namespace LD
{

class RenderPassResources : public RenderResources
{
public:
    void Startup(RDevice device);
    void Cleanup();

    RPass GetSwapChainRenderPass();
    GBufferPass& GetGBufferPass();

private:
    RPass mSwapChainRenderPass;
    GBufferPass mGBufferPass;
};

} // namespace LD