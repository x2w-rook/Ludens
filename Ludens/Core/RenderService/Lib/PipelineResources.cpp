#include "Core/DSA/Include/Array.h"
#include "Core/RenderService/Lib/PipelineResources.h"
#include "Core/RenderService/Lib/RenderPassResources.h"
#include "Core/RenderService/Lib/BindingGroupResources.h"

namespace LD
{

void PipelineResources::Startup(RDevice device, RenderPassResources* passRes, BindingGroupResources* groupRes)
{
    LD_DEBUG_ASSERT(device && passRes && groupRes);
    mDevice = device;
    mPassRes = passRes;
    mGroupRes = groupRes;
}

void PipelineResources::Cleanup()
{
    if (mRect)
        mRect.Cleanup();

    if (mGBuffer)
        mGBuffer.Cleanup();

    if (mDeferredBlinnPhong)
        mDeferredBlinnPhong.Cleanup();
    
    mGroupRes = nullptr;
    mPassRes = nullptr;
    mDevice.ResetHandle();
}

GBufferPipeline& PipelineResources::GetGBufferPipeline()
{
    if (!mGBuffer)
    {
        Array<RBindingGroupLayout, 2> groupLayout;
        groupLayout[0] = mGroupRes->GetViewportBGL();
        groupLayout[1] = mGroupRes->GetMaterialBGL();

        GBufferPipelineInfo pipelineI;
        pipelineI.Device = mDevice;
        pipelineI.RenderPass = (RPass)mPassRes->GetGBufferPass();
        pipelineI.GBufferPipelineLayout.GroupLayouts = groupLayout.GetView();

        mGBuffer.Startup(pipelineI);
        LD_DEBUG_ASSERT(mGBuffer);
    }

    return mGBuffer;
}

RectPipeline& PipelineResources::GetRectPipeline()
{
    if (!mRect)
    {
        Array<RBindingGroupLayout, 2> groupLayout;
        groupLayout[0] = mGroupRes->GetViewportBGL();
        groupLayout[1] = mGroupRes->GetRectBGL();

        RectPipelineInfo pipelineI;
        pipelineI.Device = mDevice;
        pipelineI.RenderPass = (RPass)mPassRes->GetSwapChainRenderPass();
        pipelineI.RectPipelineLayout.GroupLayouts = groupLayout.GetView();

        mRect.Startup(pipelineI);
        LD_DEBUG_ASSERT(mRect);
    }

    return mRect;
}

DeferredBlinnPhongPipeline& PipelineResources::GetDeferredBlinnPhongPipeline()
{
    if (!mDeferredBlinnPhong)
    {
        Array<RBindingGroupLayout, 2> groupLayout;
        groupLayout[0] = mGroupRes->GetFrameStaticBGL();
        groupLayout[1] = mGroupRes->GetViewportBGL();

        DeferredBlinnPhongPipelineInfo pipelineI;
        pipelineI.Device = mDevice;
        pipelineI.RenderPass = (RPass)mPassRes->GetSwapChainRenderPass();
        pipelineI.PipelineLayout.GroupLayouts = groupLayout.GetView();

        mDeferredBlinnPhong.Startup(pipelineI);
        LD_DEBUG_ASSERT(mDeferredBlinnPhong);
    }

    return mDeferredBlinnPhong;
}

} // namespace LD