#pragma once

#include <cstddef>
#include "Core/RenderBase/Include/RTypes.h"

namespace LD
{

enum class RTextureFormat
{
    Undefined = 0,
    R8,
    BGRA8,
    RGBA8,
    RGBA16F,
    D24S8,
    D32F,
    EnumCount
};

inline bool IsColorTextureFormat(RTextureFormat format)
{
    return 0 < (int)format && (int)format < (int)RTextureFormat::D24S8;
}

inline bool IsDepthStencilTextureFormat(RTextureFormat format)
{
    return (int)RTextureFormat::D24S8 <= (int)format && (int)format < (int)RTextureFormat::EnumCount;
}

size_t GetTextureFormatPixelSize(RTextureFormat format);

enum RTextureUsageFlags : u8
{
    // this texture can be used as a frame buffer attachment
    TEXTURE_USAGE_FRAME_BUFFER_ATTACHMENT_BIT = (1 << 0)
};

enum class RTextureType
{
    Texture2D = 0,
};

struct RTextureInfo
{
    RTextureType Type;
    RTextureFormat Format;
    RTextureUsageFlags TextureUsage;
    u32 Width;
    u32 Height;
    const void* Data; // pixel data
    size_t Size = 0;  // pixel data byte size
};

struct RTextureBase;
struct RTextureGL;
struct RTextureVK;

/// texture interface and handle
class RTexture : public RHandle<RTextureBase>
{
};

} // namespace LD