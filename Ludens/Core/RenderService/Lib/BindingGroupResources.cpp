#include "Core/RenderFX/Include/Groups/FrameStaticGroup.h"
#include "Core/RenderFX/Include/Groups/ViewportGroup.h"
#include "Core/RenderFX/Include/Groups/MaterialGroup.h"
#include "Core/RenderFX/Include/Groups/RectGroup.h"
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
}

void BindingGroupResources::Cleanup()
{
    if (mFrameStaticGroup)
        mFrameStaticGroup.Cleanup();

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

} // namespace LD