#include "Core/RenderService/Lib/RenderContext.h"

#define RECT_BATCH_CAPACITY 1024

namespace LD
{

namespace Embed
{

void GetDMSans_Regular(unsigned int* size, const char** data);

} // namespace Embed

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
        // ScreenViewportGroup.BindGBuffer(Textures.GetWhitePixel());
        // ScreenViewportGroup.BindSSAOTexture(Textures.GetWhitePixel());

        SSAOGroup& ssaoGroup = BindingGroups.GetSSAOGroup();
        ssaoGroup.BindSSAOTexture(DefaultSSAOBuffer.GetTexture());

         RBufferInfo info;
        info.Type = RBufferType::VertexBuffer;
        info.MemoryUsage = RMemoryUsage::Immutable;
        info.Data = sQuadVertices;
        info.Size = sizeof(sQuadVertices);
        Device.CreateBuffer(QuadVBO, info);

        unsigned int ttfSize;
        const char* ttfData;
        Embed::GetDMSans_Regular(&ttfSize, &ttfData);

        FontTTFInfo ttfI;
        ttfI.Name = "DMSansRegular";
        ttfI.TTFData = (const void*)ttfData;
        ttfI.TTFSize = (size_t)ttfSize;
        ttfI.PixelSize = 24.0f;
        DefaultFontTTF = MakeRef<FontTTF>(ttfI);

        RFontAtlasInfo atlasI;
        atlasI.Device = Device;
        atlasI.FontData = DefaultFontTTF;
        DefaultFontAtlas.Startup(atlasI);

        // TODO: flush when batch capacity is full
        DefaultRectBatch.Startup(Device, RECT_BATCH_CAPACITY);
        DefaultRectGroup.Startup(Device, BindingGroups.GetRectBGL());

        for (int i = 0; i < 16; i++)
            DefaultRectGroup.BindTexture(Textures.GetWhitePixel(), i);

        DefaultRectGroup.BindTexture(DefaultFontAtlas.GetAtlas(), 1);
    }
}

void RenderContext::Cleanup()
{
    Device.WaitIdle();

    {
        DefaultRectGroup.Cleanup();
        DefaultRectBatch.Cleanup();
        DefaultFontAtlas.Cleanup();
        DefaultFontTTF = nullptr;
        Device.DeleteBuffer(QuadVBO);
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