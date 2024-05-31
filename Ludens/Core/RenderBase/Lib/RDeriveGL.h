#pragma once

#include <glad/glad.h>
#include "Core/Header/Include/Error.h"
#include "Core/RenderBase/Include/GL/GLVertex.h"
#include "Core/RenderBase/Include/GL/GLFrameBuffer.h"
#include "Core/RenderBase/Include/RDevice.h"
#include "Core/RenderBase/Include/RBuffer.h"
#include "Core/RenderBase/Include/RShader.h"
#include "Core/RenderBase/Include/RPipeline.h"
#include "Core/RenderBase/Include/RTexture.h"
#include "Core/RenderBase/Include/RFrameBuffer.h"

namespace LD
{

inline GLenum DeriveGLIndexType(RIndexType type)
{
    switch (type)
    {
    case RIndexType::u32:
        return GL_UNSIGNED_INT;
    case RIndexType::u16:
        return GL_UNSIGNED_SHORT;
    }

    LD_DEBUG_UNREACHABLE;
    return GL_INVALID_ENUM;
}

inline GLenum DeriveGLTarget(RBufferType type)
{
    switch (type)
    {
    case RBufferType::VertexBuffer:
        return GL_ARRAY_BUFFER;
    case RBufferType::IndexBuffer:
        return GL_ELEMENT_ARRAY_BUFFER;
    case RBufferType::UniformBuffer:
        return GL_UNIFORM_BUFFER;
    }

    LD_DEBUG_UNREACHABLE;
    return GL_INVALID_ENUM;
}

inline GLSLType DeriveGLSLType(RDataType type)
{
    switch (type)
    {
    case RDataType::Float:
        return GLSLType::Float;
    case RDataType::Vec2:
        return GLSLType::Vec2;
    case RDataType::Vec3:
        return GLSLType::Vec3;
    case RDataType::Vec4:
        return GLSLType::Vec4;
    }

    LD_DEBUG_UNREACHABLE;
    return GLSLType::Float;
}

inline GLenum DeriveGLPrimitiveTopology(RPrimitiveTopology topology)
{
    switch (topology)
    {
    case RPrimitiveTopology::TriangleList:
        return GL_TRIANGLES;
    }

    LD_DEBUG_UNREACHABLE;
    return GL_INVALID_ENUM;
}

inline void DeriveGLVertexLayout(const RVertexBufferSlot& slot, GLVertexLayout& layout)
{
    for (const RVertexAttribute& attr : slot.Attributes)
    {
        GLVertexAttribute nativeAttr{};
        nativeAttr.Location = (GLuint)attr.Location;
        nativeAttr.Type = DeriveGLSLType(attr.Type);
        nativeAttr.Normalize = attr.IsNormalized;
        layout.AddVertexAttribute(nativeAttr);
    }
}

inline GLenum DeriveGLBlendOp(RBlendMode mode)
{
    switch (mode)
    {
    case RBlendMode::Add:
        return GL_FUNC_ADD;
    default:
        break;
    }

    LD_DEBUG_UNREACHABLE;
    return GL_INVALID_ENUM;
}

inline GLenum DeriveGLBlendFactor(RBlendFactor factor)
{
    switch (factor)
    {
    case RBlendFactor::Zero:
        return GL_ZERO;
    case RBlendFactor::One:
        return GL_ONE;
    case RBlendFactor::SrcAlpha:
        return GL_SRC_ALPHA;
    case RBlendFactor::DstAlpha:
        return GL_DST_ALPHA;
    case RBlendFactor::OneMinusSrcAlpha:
        return GL_ONE_MINUS_SRC_ALPHA;
    case RBlendFactor::OneMinusDstAlpha:
        return GL_ONE_MINUS_DST_ALPHA;
    default:
        break;
    }

    LD_DEBUG_UNREACHABLE;
    return GL_INVALID_ENUM;
}

void DeriveGLTextureFormat(const RTextureFormat& format, GLenum* internalFormat, GLenum* dataFormat, GLenum* type);

} // namespace LD