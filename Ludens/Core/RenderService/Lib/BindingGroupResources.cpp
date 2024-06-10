#include "Core/RenderFX/Include/Groups/FrameStaticGroup.h"
#include "Core/RenderFX/Include/Groups/ViewportGroup.h"
#include "Core/RenderFX/Include/Groups/MaterialGroup.h"
#include "Core/RenderFX/Include/Groups/RectGroup.h"
#include "Core/RenderFX/Include/Groups/SSAOGroup.h"
#include "Core/RenderService/Lib/BindingGroupResources.h"

namespace LD
{

void BindingGroupResources::Startup(RDevice device)
{
    LD_DEBUG_ASSERT(device);
    mDevice = device;

    mFrameStaticBGL = FrameStaticGroup{}.CreateLayout(mDevice);
    mViewportBGL = ViewportGroup{}.CreateLayout(mDevice);
    mMaterialBGL = MaterialGroup{}.CreateLayout(mDevice);
    mRectBGL = RectGroup{}.CreateLayout(mDevice);
    mSSAOBGL = SSAOGroup{}.CreateLayout(mDevice);
}

void BindingGroupResources::Cleanup()
{
    if (mFrameStaticGroup)
        mFrameStaticGroup.Cleanup();

    if (mSSAOGroup)
        mSSAOGroup.Cleanup();

    mDevice.DeleteBindingGroupLayout(mSSAOBGL);
    mDevice.DeleteBindingGroupLayout(mRectBGL);
    mDevice.DeleteBindingGroupLayout(mMaterialBGL);
    mDevice.DeleteBindingGroupLayout(mViewportBGL);
    mDevice.DeleteBindingGroupLayout(mFrameStaticBGL);

    mDevice.ResetHandle();
}

FrameStaticGroup& BindingGroupResources::GetFrameStaticGroup()
{
    if (!mFrameStaticGroup)
    {
        mFrameStaticGroup.Startup(mDevice, mFrameStaticBGL);
        LD_DEBUG_ASSERT(mFrameStaticGroup);
    }

    return mFrameStaticGroup;
}

SSAOGroup& BindingGroupResources::GetSSAOGroup()
{
    if (!mSSAOGroup)
    {
        mSSAOGroup.Startup(mDevice, mSSAOBGL);
        LD_DEBUG_ASSERT(mSSAOGroup);
    }

    return mSSAOGroup;
}

} // namespace LD