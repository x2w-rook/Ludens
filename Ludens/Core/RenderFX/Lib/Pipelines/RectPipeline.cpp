#include <unordered_map>
#include "Core/Header/Include/Error.h"
#include "Core/Math/Include/Mat4.h"
#include "Core/Math/Include/Math.h"
#include "Core/DSA/Include/Array.h"
#include "Core/RenderBase/Include/RBinding.h"
#include "Core/RenderBase/Include/RTexture.h"
#include "Core/RenderBase/Include/RBuffer.h"
#include "Core/RenderBase/Include/RPipeline.h"
#include "Core/RenderBase/Include/RShader.h"
#include "Core/RenderFX/Include/Pipelines/RectPipeline.h"
#include "Core/RenderFX/Include/Groups/ViewportGroup.h"
#include "Core/RenderFX/Include/Groups/RectGroup.h"
#include "Core/Media/Include/Image.h"

namespace LD {

namespace Embed {

extern void GetRectGLVS(unsigned int* size, const char** data);
extern void GetRectGLFS(unsigned int* size, const char** data);
extern void GetRectVKVS(unsigned int* size, const char** data);
extern void GetRectVKFS(unsigned int* size, const char** data);

} // namespace Embed

static const Vec2 sUnitRectPos[] = {
    { 0.5f, 0.5f },
    { 0.5f, -0.5f },
    { -0.5f, -0.5f },
    { -0.5f, 0.5f },
};

RectBatch::RectBatch()
{
}

RectBatch::~RectBatch()
{
    LD_DEBUG_ASSERT(!mDevice);
}

void RectBatch::Startup(RDevice device, int capacity)
{
    // one rect is 4 vertices referenced by 6 indices
    int indexSequence[] = { 0, 1, 2, 2, 3, 0 };

    mDevice = device;
    mBatch.Startup(4, { 6, indexSequence }, capacity);

    RBufferInfo info{};
    info.MemoryUsage = RMemoryUsage::FrameDynamic;
    info.Type = RBufferType::VertexBuffer;
    info.Size = mBatch.GetVertexBufferSize();
    info.Data = nullptr;
    mDevice.CreateBuffer(mVertexBuffer, info);

    info.MemoryUsage = RMemoryUsage::Immutable;
    info.Type = RBufferType::IndexBuffer;
    info.Size = mBatch.GetIndexBufferSize();
    info.Data = mBatch.GetIndices();
    mDevice.CreateBuffer(mIndexBuffer, info);

    mRectCount = 0;
}

void RectBatch::Cleanup()
{
    mDevice.DeleteBuffer(mIndexBuffer);
    mDevice.DeleteBuffer(mVertexBuffer);
    mBatch.Cleanup();
    mDevice.ResetHandle();
}

void RectBatch::Reset()
{
    mBatch.Reset();
    mRectCount = -1;
}

bool RectBatch::AddCustom(const RectVertex* vertices)
{
    if (mBatch.GetElementCapacity() == mBatch.GetElementCount())
        return false;

    bool ok = mBatch.AddElement(vertices);
    LD_DEBUG_ASSERT(ok);
    return ok;
}

bool RectBatch::AddRectOutline(const Rect2D& rect, Vec4 color, float lineWidth)
{
    if (mBatch.GetElementCapacity() - mBatch.GetElementCount() < 4)
        return false;

    Rect2D borderL{ rect.x, rect.y, lineWidth, rect.h };
    Rect2D borderR{ rect.x + rect.w - lineWidth, rect.y, lineWidth, rect.h };
    Rect2D borderT{ rect.x, rect.y, rect.w, lineWidth };
    Rect2D borderB{ rect.x, rect.y + rect.h - lineWidth, rect.w, lineWidth };

    bool ok = AddRectFilled(borderL, color) && AddRectFilled(borderR, color) && AddRectFilled(borderT, color) &&
              AddRectFilled(borderB, color);
    LD_DEBUG_ASSERT(ok);
    return ok;
}

bool RectBatch::AddRectFilled(const Rect2D& rect, Vec4 color)
{
    RectVertex vertex[4];

    if (mBatch.GetElementCapacity() == mBatch.GetElementCount())
        return false;

    for (int i = 0; i < 4; i++)
    {
        float x = sUnitRectPos[i].x + 0.5f;
        float y = sUnitRectPos[i].y + 0.5f;

        vertex[i].Color = color;
        vertex[i].Position.x = (x * rect.w) + rect.x;
        vertex[i].Position.y = (y * rect.h) + rect.y;
        vertex[i].TexID = 0.0f; // white pixel texture
    }

    vertex[0].TexUV = { 1.0f, 0.0f };
    vertex[1].TexUV = { 1.0f, 1.0f };
    vertex[2].TexUV = { 0.0f, 1.0f };
    vertex[3].TexUV = { 0.0f, 0.0f };

    bool ok = mBatch.AddElement(vertex);
    LD_DEBUG_ASSERT(ok);
    return ok;
}

bool RectBatch::AddTexture(const Rect2D& rect, const Rect2D& texRegion, Vec2 texSize, Vec4 color, int texID)
{
    RectVertex vertex[4];

    float u0 = texRegion.x / texSize.x;
    float v0 = texRegion.y / texSize.y;
    float u1 = (texRegion.x + texRegion.w) / texSize.x;
    float v1 = (texRegion.y + texRegion.h) / texSize.y;

    for (int i = 0; i < 4; i++)
    {
        float x = sUnitRectPos[i].x + 0.5f;
        float y = sUnitRectPos[i].y + 0.5f;

        vertex[i].Color = color;
        vertex[i].Position.x = (x * rect.w) + rect.x;
        vertex[i].Position.y = (y * rect.h) + rect.y;
        vertex[i].TexID = (float)texID;
    }

    vertex[0].TexUV = { u1, v0 };
    vertex[1].TexUV = { u1, v1 };
    vertex[2].TexUV = { u0, v1 };
    vertex[3].TexUV = { u0, v0 };

    bool ok = mBatch.AddElement(vertex);
    LD_DEBUG_ASSERT(ok);
    return ok;
}

bool RectBatch::AddGlyph(const Vec2& cursor, const FontGlyph& glyph, float scale, Vec4 color, int texID)
{
    if (mBatch.GetElementCapacity() == mBatch.GetElementCount())
        return false;

    RectVertex vertex[4];
    float u0 = glyph.RectUV.x;
    float v0 = glyph.RectUV.y;
    float u1 = glyph.RectUV.x + glyph.RectUV.w;
    float v1 = glyph.RectUV.y + glyph.RectUV.h;

    // glyph bounding box top left corner, derived from baseline cursor and glyph bearing
    float gx = cursor.x + glyph.BearingX * scale;
    float gy = cursor.y - glyph.BearingY * scale;

    // glyph rendered size
    float gw = glyph.RectXY.w * scale;
    float gh = glyph.RectXY.h * scale;

    vertex[0].Color = color;
    vertex[0].TexID = (float)texID;
    vertex[0].TexUV = { u0, v0 };
    vertex[0].Position = { gx, gy };
    vertex[1].Color = color;
    vertex[1].TexID = (float)texID;
    vertex[1].TexUV = { u0, v1 };
    vertex[1].Position = { gx, gy + gh };
    vertex[2].Color = color;
    vertex[2].TexID = (float)texID;
    vertex[2].TexUV = { u1, v1 };
    vertex[2].Position = { gx + gw, gy + gh };
    vertex[3].Color = color;
    vertex[3].TexID = (float)texID;
    vertex[3].TexUV = { u1, v0 };
    vertex[3].Position = { gx + gw, gy };

    bool ok = mBatch.AddElement(vertex);
    LD_DEBUG_ASSERT(ok);
    return ok;
}


int RectBatch::GetRectCount()
{
    return mBatch.GetElementCount();
}

void RectBatch::GetBuffers(RBuffer& vertexBuffer, RBuffer& indexBuffer)
{
    int rectCount = mBatch.GetElementCount();

    if (rectCount != mRectCount)
    {
        mVertexBuffer.SetData(0, sizeof(RectVertex) * 4 * rectCount, mBatch.GetVertices());
        mRectCount = rectCount;
    }

    vertexBuffer = mVertexBuffer;
    indexBuffer = mIndexBuffer;
}

RectPipeline::RectPipeline()
{
}

RectPipeline::~RectPipeline()
{
    LD_DEBUG_ASSERT(!mDevice);
}

void RectPipeline::Startup(const RectPipelineInfo& info)
{
    mDevice = info.Device;
    RBackend backend = mDevice.GetBackend();

    RVertexBufferSlot slot{};
    Array<RVertexAttribute, 4> attributes{
        RVertexAttribute{ 0, RDataType::Vec2, false },  // Pos
        RVertexAttribute{ 1, RDataType::Vec2, false },  // TexUV
        RVertexAttribute{ 2, RDataType::Vec4, false },  // Color
        RVertexAttribute{ 3, RDataType::Float, false }, // TexID
    };
    slot.PollRate = RAttributePollRate::PerVertex;
    slot.Attributes = { attributes.Size(), attributes.Data() };

    const char* vsData;
    unsigned int vsSize;
    const char* fsData;
    unsigned int fsSize;

    if (backend == RBackend::Vulkan)
    {
        Embed::GetRectVKVS(&vsSize, &vsData);
        Embed::GetRectVKFS(&fsSize, &fsData);
    }
    else
    {
        Embed::GetRectGLVS(&vsSize, &vsData);
        Embed::GetRectGLFS(&fsSize, &fsData);
    }

    RShaderInfo vertexSI;
    vertexSI.SourceType = RShaderSourceType::SPIRV;
    vertexSI.Type = RShaderType::VertexShader;
    vertexSI.Data = vsData;
    vertexSI.Size = vsSize;
    mDevice.CreateShader(mRectVS, vertexSI);

    RShaderInfo fragmentSI;
    fragmentSI.SourceType = RShaderSourceType::SPIRV;
    fragmentSI.Type = RShaderType::FragmentShader;
    fragmentSI.Data = fsData;
    fragmentSI.Size = fsSize;
    mDevice.CreateShader(mRectFS, fragmentSI);

    RPipelineInfo pipelineI{};
    pipelineI.Name = "RectPipeline";
    pipelineI.VertexLayout.Slots = { 1, &slot };
    pipelineI.VertexShader = mRectVS;
    pipelineI.FragmentShader = mRectFS;
    pipelineI.PrimitiveTopology = RPrimitiveTopology::TriangleList;
    pipelineI.PipelineLayout = info.RectPipelineLayout;
    pipelineI.RenderPass = info.RenderPass;
    pipelineI.DepthStencilState.DepthTestEnabled = false;
    pipelineI.BlendState.BlendEnabled = true;
    pipelineI.BlendState.ColorSrcFactor = RBlendFactor::SrcAlpha;
    pipelineI.BlendState.ColorDstFactor = RBlendFactor::OneMinusSrcAlpha;
    pipelineI.BlendState.ColorBlendMode = RBlendMode::Add;
    pipelineI.BlendState.AlphaSrcFactor = RBlendFactor::One;
    pipelineI.BlendState.AlphaDstFactor = RBlendFactor::Zero;
    pipelineI.BlendState.AlphaBlendMode = RBlendMode::Add;
    mDevice.CreatePipeline(mHandle, pipelineI);
}

void RectPipeline::Cleanup()
{
    mDevice.DeleteShader(mRectFS);
    mDevice.DeleteShader(mRectVS);
    mDevice.DeletePipeline(mHandle);
    mDevice.ResetHandle();
}

RPipelineLayoutData RectPipeline::GetLayoutData() const
{
    RBindingGroupLayoutData group0 = ViewportGroup{}.GetLayoutData();
    RBindingGroupLayoutData group1 = RectGroup{}.GetLayoutData();
    RPipelineLayoutData data{};
    data.GroupLayouts = { group0, group1 };
    
    return data;
}

} // namespace LD
