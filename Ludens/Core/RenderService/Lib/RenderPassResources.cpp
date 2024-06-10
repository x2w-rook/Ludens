#include "Core/RenderService/Lib/RenderPassResources.h"

namespace LD
{
void RenderPassResources::Startup(RDevice device)
{
    LD_DEBUG_ASSERT(device);
    mDevice = device;
}

void RenderPassResources::Cleanup()
{
    if (mGBufferPass)
        mGBufferPass.Cleanup();

    if (mSSAOPass)
        mSSAOPass.Cleanup();

    mDevice.ResetHandle();
}

RPass RenderPassResources::GetSwapChainRenderPass()
{
    if (!mSwapChainRenderPass)
    {
        mDevice.GetSwapChainRenderPass(mSwapChainRenderPass);
        LD_DEBUG_ASSERT(mSwapChainRenderPass);
    }

    return mSwapChainRenderPass;
}

GBufferPass& RenderPassResources::GetGBufferPass()
{
    if (!mGBufferPass)
    {
        GBufferPassInfo passI;
        passI.Device = mDevice;
        passI.PositionFormat = RTextureFormat::RGBA16F;
        passI.NormalFormat = RTextureFormat::RGBA16F;
        passI.AlbedoFormat = RTextureFormat::RGBA8;
        passI.DepthStencilFormat = RTextureFormat::D32F;

        mGBufferPass.Startup(passI);
        LD_DEBUG_ASSERT(mGBufferPass);
    }

    return mGBufferPass;
}

SSAOPass& RenderPassResources::GetSSAOPass()
{
    if (!mSSAOPass)
    {
        mSSAOPass.Startup(mDevice);
        LD_DEBUG_ASSERT(mSSAOPass);
    }

    return mSSAOPass;
}

} // namespace LD