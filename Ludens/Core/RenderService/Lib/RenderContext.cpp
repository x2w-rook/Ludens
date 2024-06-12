#include "Core/RenderService/Lib/RenderContext.h"

#define RECT_BATCH_CAPACITY  1024

namespace LD
{

// clang-format off
static float sQuadVertices[]{
     1.0f,  1.0f,  1.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f,
    -1.0f,  1.0f,  0.0f, 1.0f,
};
// clang-format on

void RenderContext::Startup(RDevice device, int viewportWidth, int viewportHeight)
{
    LD_DEBUG_ASSERT(device);
    Device = device;

    ViewportWidth = viewportWidth;
    ViewportHeight = viewportHeight;

    Passes.Startup(device);
    FrameBuffers.Startup(Device, &Passes);
    BindingGroups.Startup(Device);
    Pipelines.Startup(Device, &Passes, &BindingGroups);
    Textures.Startup(Device);

    {
        FrameBuffers.CreateGBuffer(DefaultGBuffer, ViewportWidth, ViewportHeight);
        FrameBuffers.CreateSSAOBuffer(DefaultSSAOBuffer, ViewportWidth, ViewportHeight, (RPass)Passes.GetSSAOPass());
        FrameBuffers.CreateSSAOBuffer(DefaultSSAOBlurBuffer, ViewportWidth, ViewportHeight,
                                      (RPass)Passes.GetSSAOPass());
        
        WorldViewportGroup.Startup(Device, BindingGroups.GetViewportBGL());
        WorldViewportGroup.BindGBuffer(DefaultGBuffer);
        WorldViewportGroup.BindSSAOTexture(DefaultSSAOBlurBuffer.GetTexture());

        ScreenViewportGroup.Startup(Device, BindingGroups.GetViewportBGL());
        //ScreenViewportGroup.BindGBuffer(Textures.GetWhitePixel());
        //ScreenViewportGroup.BindSSAOTexture(Textures.GetWhitePixel());

        SSAOGroup& ssaoGroup = BindingGroups.GetSSAOGroup();
        ssaoGroup.BindSSAOTexture(DefaultSSAOBuffer.GetTexture());

        DefaultRectBatch.Startup(Device, RECT_BATCH_CAPACITY);
        DefaultRectGroup.Startup(Device, BindingGroups.GetRectBGL());

        for (int i = 0; i < 16; i++)
            DefaultRectGroup.BindTexture(Textures.GetWhitePixel(), i);

        RBufferInfo info;
        info.Type = RBufferType::VertexBuffer;
        info.MemoryUsage = RMemoryUsage::Immutable;
        info.Data = sQuadVertices;
        info.Size = sizeof(sQuadVertices);
        Device.CreateBuffer(QuadVBO, info);
    }
}

void RenderContext::Cleanup()
{
    Device.WaitIdle();

    {
        Device.DeleteBuffer(QuadVBO);
        DefaultRectGroup.Cleanup();
        DefaultRectBatch.Cleanup();
        ScreenViewportGroup.Cleanup();
        WorldViewportGroup.Cleanup();
        DefaultGBuffer.Cleanup();
        DefaultSSAOBuffer.Cleanup();
        DefaultSSAOBlurBuffer.Cleanup();
    }

    Textures.Cleanup();
    Pipelines.Cleanup();
    BindingGroups.Cleanup();
    FrameBuffers.Cleanup();
    Passes.Cleanup();

    Device.ResetHandle();
}

void RenderContext::OnViewportResize(int viewportWidth, int viewportHeight)
{
    ViewportWidth = viewportWidth;
    ViewportHeight = viewportHeight;

    // recreate swapchain
    Device.ResizeViewport(ViewportWidth, ViewportHeight);

    if (DefaultGBuffer)
        DefaultGBuffer.Cleanup();
    FrameBuffers.CreateGBuffer(DefaultGBuffer, ViewportWidth, ViewportHeight);

    if (DefaultSSAOBuffer)
        DefaultSSAOBuffer.Cleanup();
    FrameBuffers.CreateSSAOBuffer(DefaultSSAOBuffer, ViewportWidth, ViewportHeight, (RPass)Passes.GetSSAOPass());

    if (DefaultSSAOBlurBuffer)
        DefaultSSAOBlurBuffer.Cleanup();
    FrameBuffers.CreateSSAOBuffer(DefaultSSAOBlurBuffer, ViewportWidth, ViewportHeight, (RPass)Passes.GetSSAOPass());

    // make gbuffer results visible from the viewport group
    WorldViewportGroup.BindGBuffer(DefaultGBuffer);

    // make ssao results visible from the viewport group
    SSAOGroup& ssaoGroup = BindingGroups.GetSSAOGroup();
    ssaoGroup.BindSSAOTexture(DefaultSSAOBuffer.GetTexture());
    WorldViewportGroup.BindSSAOTexture(DefaultSSAOBlurBuffer.GetTexture());
}

} // namespace LD