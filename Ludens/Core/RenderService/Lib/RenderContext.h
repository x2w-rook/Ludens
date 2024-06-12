#pragma once

#include "Core/RenderBase/Include/RDevice.h"
#include "Core/RenderFX/Include/Groups/ViewportGroup.h"
#include "Core/RenderFX/Include/Groups/RectGroup.h"
#include "Core/RenderFX/Include/FrameBuffers/GBuffer.h"
#include "Core/RenderFX/Include/FrameBuffers/SSAOBuffer.h"
#include "Core/RenderService/Lib/RenderPassResources.h"
#include "Core/RenderService/Lib/FrameBufferResources.h"
#include "Core/RenderService/Lib/BindingGroupResources.h"
#include "Core/RenderService/Lib/PipelineResources.h"
#include "Core/RenderService/Lib/TextureResources.h"

namespace LD
{

/// internal resources and state of the renderer
struct RenderContext
{
    void Startup(RDevice device, int viewportWidth, int viewportHeight);
    void Cleanup();

    void OnViewportResize(int viewportWidth, int viewportHeight);

    bool HasBeginViewport;
    bool HasBeginFrame;
    int ViewportWidth;
    int ViewportHeight;

    RDevice Device;
    RenderPassResources Passes;
    FrameBufferResources FrameBuffers;
    BindingGroupResources BindingGroups;
    PipelineResources Pipelines;
    TextureResources Textures;

    RBuffer QuadVBO;
    GBuffer DefaultGBuffer;
    SSAOBuffer DefaultSSAOBuffer;
    SSAOBuffer DefaultSSAOBlurBuffer;
    RectBatch DefaultRectBatch;
    RectGroup DefaultRectGroup;
    ViewportGroup WorldViewportGroup;
    ViewportGroup ScreenViewportGroup;
};

} // namespace LD