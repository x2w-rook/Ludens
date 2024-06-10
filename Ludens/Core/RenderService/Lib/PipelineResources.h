#pragma once

#include "Core/RenderFX/Include/Pipelines/GBufferPipeline.h"
#include "Core/RenderFX/Include/Pipelines/RectPipeline.h"
#include "Core/RenderFX/Include/Pipelines/DeferredBlinnPhongPipeline.h"
#include "Core/RenderFX/Include/Pipelines/DeferredSSAOPipeline.h"
#include "Core/RenderService/Lib/RenderResources.h"

namespace LD
{

class RenderPassResources;
class BindingGroupResources;

class PipelineResources : public RenderResources
{
public:
    void Startup(RDevice device, RenderPassResources* passRes, BindingGroupResources* groupRes);
    void Cleanup();

    GBufferPipeline& GetGBufferPipeline();

    RectPipeline& GetRectPipeline();

    DeferredBlinnPhongPipeline& GetDeferredBlinnPhongPipeline();

    DeferredSSAOPipeline& GetDeferredSSAOPipeline();

private:
    RenderPassResources* mPassRes;
    BindingGroupResources* mGroupRes;
    GBufferPipeline mGBuffer;
    RectPipeline mRect;
    DeferredBlinnPhongPipeline mDeferredBlinnPhong;
    DeferredSSAOPipeline mDeferredSSAO;
};

} // namespace LD