#include "Core/RenderBase/Include/VK/VKInfo.h"
#include "Core/RenderBase/Lib/RTextureVK.h"
#include "Core/RenderBase/Lib/RDeviceVK.h"
#include "Core/RenderBase/Lib/RDeriveVK.h"

namespace LD
{

RTextureVK::RTextureVK()
{
}

RTextureVK::~RTextureVK()
{
    LD_DEBUG_ASSERT(ID == 0);
}

void RTextureVK::Startup(RTexture& textureH, Ref<VKImageView> view, RDeviceVK& device)
{
    RTextureBase::Startup(textureH, (RDeviceBase*)&device);
    VKContext& vkContext = device.Context;
    VKDevice& vkDevice = vkContext.GetDevice();

    // the actual image and image view lives somewhere else
    UseExternalImage = true;

    LD_DEBUG_ASSERT(view != nullptr);
    ImageView = view;

    // create sampler
    {
        const VkPhysicalDeviceLimits& deviceLimits = vkDevice.GetPhysicalDevice().GetLimits();
        VkSamplerCreateInfo samplerCI = VKInfo::SamplerCreate(
            VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_REPEAT, deviceLimits.maxSamplerAnisotropy);

        Sampler.Startup(vkDevice, samplerCI);
    }
}

void RTextureVK::Startup(RTexture& textureH, const RTextureInfo& info, RDeviceVK& device)
{
    RTextureBase::Startup(textureH, info, (RDeviceBase*)&device);
    VKContext& vkContext = device.Context;
    VKDevice& vkDevice = vkContext.GetDevice();
    VkFormat imageFormat = DeriveVKImageFormat(info.Format);
    bool hasDepth = VKFormat::HasDepthComponent(imageFormat);
    bool hasStencil = VKFormat::HasStencilComponent(imageFormat);

    ImageView = nullptr;
    UseExternalImage = false;

    // create 2D image from data
    {
        LD_DEBUG_ASSERT(info.Type == RTextureType::Texture2D);

        const void* layers[1] = { info.Data };

        VkImageUsageFlags imageUsage = VK_IMAGE_USAGE_SAMPLED_BIT;

        if (info.Size > 0 && info.Data)
        {
            imageUsage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        }

        if (info.TextureUsage & TEXTURE_USAGE_FRAME_BUFFER_ATTACHMENT_BIT)
        {
            imageUsage |= (hasDepth || hasStencil ? VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT
                                                  : VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
        }

        VKImageInfo imageI{};
        imageI.MemoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
        imageI.CreateInfo =
            VKInfo::Image2DCreate(imageFormat, { info.Width, info.Height }, imageUsage, VK_SHARING_MODE_EXCLUSIVE);

        Image.Startup(vkDevice, imageI);

        if (imageUsage & VK_IMAGE_USAGE_TRANSFER_DST_BIT)
        {
            Image.StageData(1, info.Size, layers, device.TransferCommandPool, vkDevice.GetTransferQueue());
        }
    }

    // create image view
    {
        VkImageAspectFlags aspectFlags = 0;
        if (hasStencil)
            aspectFlags |= VK_IMAGE_ASPECT_STENCIL_BIT;
        if (hasDepth)
            aspectFlags |= VK_IMAGE_ASPECT_DEPTH_BIT;
        if (aspectFlags == 0)
            aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;

        VkImageViewCreateInfo imageViewCI =
            VKInfo::ImageViewCreate(VK_IMAGE_VIEW_TYPE_2D, Image.GetHandle(), imageFormat, aspectFlags);

        ImageView = MakeRef<VKImageView>();
        ImageView->Startup(vkDevice, imageViewCI);
    }

    // create sampler
    {
        const VkPhysicalDeviceLimits& deviceLimits = vkDevice.GetPhysicalDevice().GetLimits();
        VkSamplerCreateInfo samplerCI = VKInfo::SamplerCreate(
            VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_REPEAT, deviceLimits.maxSamplerAnisotropy);

        Sampler.Startup(vkDevice, samplerCI);
    }
}

void RTextureVK::Cleanup(RTexture& textureH)
{
    RTextureBase::Cleanup(textureH);

    Sampler.Cleanup();

    if (!UseExternalImage)
    {
        ImageView->Cleanup();
        Image.Cleanup();
    }

    ImageView = nullptr;
}

} // namespace LD