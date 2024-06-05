#include "Core/Header/Include/Error.h"
#include "Core/RenderBase/Include/GL/GLTexture.h"
#include "Core/RenderBase/Lib/RTextureGL.h"
#include "Core/RenderBase/Lib/RDeviceGL.h"
#include "Core/RenderBase/Lib/RDeriveGL.h"

namespace LD
{

RTextureGL::RTextureGL()
{
}

RTextureGL::~RTextureGL()
{
    LD_DEBUG_ASSERT(ID == 0);
}

void RTextureGL::Startup(RTexture& textureH, const RTextureInfo& info, RDeviceGL& device)
{
    RTextureBase::Startup(textureH, info, &device);

    LD_DEBUG_ASSERT(info.Type == RTextureType::Texture2D);

    Target = GL_TEXTURE_2D;

    if (Target == GL_TEXTURE_2D)
    {
        GLenum minFilter, magFilter, addrMode;
        DeriveGLSamplerAddressMode(info.Sampler.AddressMode, addrMode);
        DeriveGLSamplerFilter(info.Sampler.MinFilter, minFilter);
        DeriveGLSamplerFilter(info.Sampler.MagFilter, magFilter);

        GLTexture2DInfo glInfo{};
        glInfo.Width = info.Width;
        glInfo.Height = info.Height;
        glInfo.Data = info.Data;
        glInfo.MinFilter = minFilter;
        glInfo.MagFilter = magFilter;
        glInfo.AddressModeS = addrMode;
        glInfo.AddressModeT = addrMode;
        DeriveGLTextureFormat(info.Format, &glInfo.InternalFormat, &glInfo.DataFormat, &glInfo.DataType);
        Texture2D.Startup(device.Context, glInfo);
    }
    else
        LD_DEBUG_UNREACHABLE;
}

void RTextureGL::Cleanup(RTexture& textureH)
{
    RTextureBase::Cleanup(textureH);

    Texture2D.Cleanup();
}

void RTextureGL::Bind(int unit)
{
    if (Target == GL_TEXTURE_2D)
        Texture2D.Bind(unit);
    else
        LD_DEBUG_UNREACHABLE;
}

} // namespace LD