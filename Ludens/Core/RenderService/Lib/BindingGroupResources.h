#pragma once

#include "Core/RenderBase/Include/RBinding.h"
#include "Core/RenderFX/Include/Groups/FrameStaticGroup.h"
#include "Core/RenderService/Lib/RenderResources.h"

namespace LD
{

class BindingGroupResources : public RenderResources
{
public:

    void Startup(RDevice device);
    void Cleanup();

    FrameStaticGroup& GetFrameStaticGroup();

    inline RBindingGroupLayout GetFrameStaticBGL()
    {
        return mFrameStaticBGL;
    }

    inline RBindingGroupLayout GetViewportBGL()
    {
        return mViewportBGL;
    }

    inline RBindingGroupLayout GetMaterialBGL()
    {
        return mMaterialBGL;
    }

    inline RBindingGroupLayout GetRectBGL()
    {
        return mRectBGL;
    }

private:
    FrameStaticGroup mFrameStaticGroup;
    RBindingGroupLayout mFrameStaticBGL;
    RBindingGroupLayout mViewportBGL;
    RBindingGroupLayout mMaterialBGL;
    RBindingGroupLayout mRectBGL;
};

} // namespace LD