#include <unordered_map>
#include <utility>
#include "Core/Math/Include/Mat3.h"
#include "Core/DSA/Include/Array.h"
#include "Core/Application/Include/Application.h"
#include "Core/RenderBase/Include/RPipeline.h"
#include "Core/RenderBase/Include/RShader.h"
#include "Core/RenderFX/Include/RMesh.h"
#include "Core/RenderFX/Include/Groups/ViewportGroup.h"
#include "Core/RenderFX/Include/Groups/GBufferGroup.h"
#include "Core/RenderService/Lib/RenderPassResources.h"
#include "Core/RenderService/Lib/FrameBufferResources.h"
#include "Core/RenderService/Lib/BindingGroupResources.h"
#include "Core/RenderService/Lib/PipelineResources.h"
#include "Core/RenderService/Lib/TextureResources.h"
#include "Core/RenderService/Include/RenderService.h"

namespace LD
{

struct MeshResource
{
    RMesh Mesh;
    RBuffer InstanceTransforms;
};

struct DrawList
{
    Vec3 ViewPos;
    Mat4 ViewMat;
    Mat4 ProjMat;
    Vector<std::pair<RRID, Mat4>> Meshes;
};

static bool sHasBeginFrame;
static bool sHasBeginViewport;
static RDevice sDevice;
static RenderPassResources sPasses;
static FrameBufferResources sFrameBuffers;
static BindingGroupResources sBindingGroups;
static PipelineResources sPipelines;
static TextureResources sTextures;
static GBuffer sGBuffer;
static RRID sDirectionalLight;
static FrameStaticLightingUBO sLightingUBO;
static ViewportGroup sViewportGroup;
static GBufferGroup sGBufferGroup;
static std::unordered_map<RRID, MeshResource> sMeshes;
static RBuffer sQuadVBO;
static Vector<DrawList> sDrawLists;

static float sQuadVertices[]{
    1.0f,  1.0f,  1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f,  -1.0f, 1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f, 0.0f, 1.0f,  1.0f,  1.0f, 1.0f, -1.0f, 1.0f,  0.0f, 1.0f,
};

static void RenderServiceCallback(const RResult& result)
{
    LD_DEBUG_ASSERT(result.Type == RResultType::Ok);
}

void RenderService::Startup(RBackend backend)
{
    int width, height;
    auto& app = Application::GetSingleton();
    app.GetWindowSize(&width, &height);

    RDeviceInfo deviceI{};
    deviceI.Backend = backend;
    deviceI.Callback = RenderServiceCallback;
    CreateRenderDevice(sDevice, deviceI);

    sPasses.Startup(sDevice);
    sFrameBuffers.Startup(sDevice, &sPasses);
    sBindingGroups.Startup(sDevice);
    sPipelines.Startup(sDevice, &sPasses, &sBindingGroups);
    sTextures.Startup(sDevice);

    {
        sFrameBuffers.CreateGBuffer(sGBuffer, width, height);
        sViewportGroup.Startup(sDevice, sBindingGroups.GetViewportBGL());
        sGBufferGroup.Startup(sDevice, sBindingGroups.GetGBufferBGL(), sGBuffer);

        RBufferInfo info;
        info.Type = RBufferType::VertexBuffer;
        info.MemoryUsage = RMemoryUsage::Immutable;
        info.Data = sQuadVertices;
        info.Size = sizeof(sQuadVertices);
        sDevice.CreateBuffer(sQuadVBO, info);
    }
}

void RenderService::Cleanup()
{
    sDevice.WaitIdle();

    {
        sDevice.DeleteBuffer(sQuadVBO);
        sGBufferGroup.Cleanup();
        sViewportGroup.Cleanup();
        sGBuffer.Cleanup();
    }

    sTextures.Cleanup();
    sPipelines.Cleanup();
    sBindingGroups.Cleanup();
    sFrameBuffers.Cleanup();
    sPasses.Cleanup();

    DeleteRenderDevice(sDevice);
}

void RenderService::BeginFrame()
{
    // adapt to application framebuffer size
    int width, height;

    auto& app = Application::GetSingleton();
    app.GetWindowPixelSize(&width, &height);

    if (width != mViewportWidth || height != mViewportHeight)
    {
        OnViewportResize(width, height);
    }

    sDrawLists.Clear();

    // upload frame static data
    FrameStaticGroup& group = sBindingGroups.GetFrameStaticGroup();
    RBuffer ubo = group.GetLightingUBO();
    ubo.SetData(0, sizeof(sLightingUBO), &sLightingUBO);

    sDevice.BeginFrame();

    sHasBeginFrame = true;
}

void RenderService::EndFrame()
{
    sHasBeginFrame = false;

    // GBuffer Pass
    {
        Array<RClearValue, 4> clearValues;
        clearValues[0].Color = { 0.0f, 0.0f, 0.0f, 1.0f };
        clearValues[1].Color = { 0.0f, 0.0f, 0.0f, 1.0f };
        clearValues[2].Color = { 0.0f, 0.0f, 0.0f, 1.0f };
        clearValues[3].DepthStencil = { 1.0f, 0 };

        RPassBeginInfo passBI;
        passBI.RenderPass = (RPass)sPasses.GetGBufferPass();
        passBI.FrameBuffer = (RFrameBuffer)sGBuffer;
        passBI.ClearValues = clearValues.GetView();
        sDevice.BeginRenderPass(passBI);

        sDevice.SetPipeline((RPipeline)sPipelines.GetGBufferPipeline());
        sDevice.SetBindingGroup(0, (RBindingGroup)sViewportGroup);

        for (DrawList& list : sDrawLists)
        {
            RBuffer& ubo = sViewportGroup.GetUBO();
            ViewportUBO viewportData;
            viewportData.PointLightStart = 0;
            viewportData.PointLightCount = 0;
            viewportData.ViewMat = list.ViewMat;
            viewportData.ProjMat = list.ProjMat;
            viewportData.ViewProjMat = list.ProjMat * list.ViewMat;
            viewportData.Size = { (float)mViewportWidth, (float)mViewportHeight };
            viewportData.ViewPos = list.ViewPos;
            ubo.SetData(0, sizeof(viewportData), &viewportData);

            for (auto& mesh : list.Meshes)
            {
                RRID id = mesh.first;
                const Mat4& modelMat = mesh.second;
                const Mat3 normalMat = Mat3::Transpose(Mat3::Inverse(Mat3(modelMat)));
                MeshResource& res = sMeshes[id];

                Array<Vec4, 6> instanceData;
                // 4x4 model matrix top 3 rows
                instanceData[0] = { modelMat[0][0], modelMat[1][0], modelMat[2][0], modelMat[3][0] };
                instanceData[1] = { modelMat[0][1], modelMat[1][1], modelMat[2][1], modelMat[3][1] };
                instanceData[2] = { modelMat[0][2], modelMat[1][2], modelMat[2][2], modelMat[3][2] };
                // 3x3 normal matrix columns
                instanceData[3] = { normalMat[0], 0.0f };
                instanceData[4] = { normalMat[1], 0.0f };
                instanceData[5] = { normalMat[2], 0.0f };

                res.InstanceTransforms.SetData(0, instanceData.ByteSize(), instanceData.Data());

                res.Mesh.Draw(
                    [&](RMesh::Batch& batch)
                    {
                        sDevice.SetBindingGroup(1, (RBindingGroup)batch.Material);
                        sDevice.SetVertexBuffer(0, batch.Vertices);
                        sDevice.SetVertexBuffer(1, res.InstanceTransforms);
                        sDevice.SetIndexBuffer(batch.Indices, RIndexType::u32);

                        RDrawIndexedInfo info{};
                        info.IndexCount = batch.IndexCount;
                        info.InstanceStart = 0;
                        info.InstanceCount = 1;
                        sDevice.DrawIndexed(info);
                    });
            }
        }

        sDevice.EndRenderPass();
    }

    // Swapchain Pass
    {
        RFrameBuffer swapChainFB;
        RPass swapChainRP;
        sDevice.GetSwapChainRenderPass(swapChainRP);
        sDevice.GetSwapChainFrameBuffer(swapChainFB);

        // NOTE: doesn't matter, colors are from albedo texture
        RClearValue clearColor;
        clearColor.Color = { 0.0f, 0.0f, 0.0f, 1.0f };

        RPassBeginInfo passBI;
        passBI.RenderPass = swapChainRP;
        passBI.FrameBuffer = swapChainFB;
        passBI.ClearValues = { 1, &clearColor };
        sDevice.BeginRenderPass(passBI);

        // Lighting
        {
            sDevice.SetPipeline((RPipeline)sPipelines.GetDeferredBlinnPhongPipeline());
            sDevice.SetBindingGroup(0, (RBindingGroup)sBindingGroups.GetFrameStaticGroup());
            sDevice.SetBindingGroup(1, (RBindingGroup)sViewportGroup);
            sDevice.SetBindingGroup(2, (RBindingGroup)sGBufferGroup);
            sDevice.SetVertexBuffer(0, sQuadVBO);

            RDrawVertexInfo drawInfo{};
            drawInfo.VertexCount = 6;
            sDevice.DrawVertex(drawInfo);
        }

        sDevice.EndRenderPass();
    }

    sDevice.EndFrame();
}

void RenderService::BeginViewport(const Vec3& viewpos, const Mat4& view, const Mat4& projection)
{
    LD_DEBUG_ASSERT(sHasBeginFrame);
    LD_DEBUG_ASSERT(!sHasBeginViewport);

    sDrawLists.PushBack({});
    DrawList& list = sDrawLists.Back();
    list.ViewPos = viewpos;
    list.ViewMat = view;
    list.ProjMat = projection;

    sHasBeginViewport = true;
}

void RenderService::EndViewport()
{
    LD_DEBUG_ASSERT(sHasBeginViewport);

    sHasBeginViewport = false;
}

void RenderService::CreateMesh(RRID& id, const Model* model)
{
    id = CUID<MeshResource>::Get();
    LD_DEBUG_ASSERT(sMeshes.find(id) == sMeshes.end());

    MeshResource& res = sMeshes[id];

    RMeshInfo meshI;
    meshI.Device = sDevice;
    meshI.MaterialBGL = sBindingGroups.GetMaterialBGL();
    meshI.Data = model;
    res.Mesh.Startup(meshI);

    // store model matrix and normal matrix of one instance
    RBufferInfo bufferI;
    bufferI.MemoryUsage = RMemoryUsage::FrameDynamic;
    bufferI.Type = RBufferType::VertexBuffer;
    bufferI.Size = sizeof(Vec4) * 6;
    bufferI.Data = nullptr;
    sDevice.CreateBuffer(res.InstanceTransforms, bufferI);
}

void RenderService::DeleteMesh(RRID id)
{
    auto& iter = sMeshes.find(id);

    if (iter == sMeshes.end())
        return;

    MeshResource& res = iter->second;
    sDevice.DeleteBuffer(res.InstanceTransforms);
    res.Mesh.Cleanup();

    sMeshes.erase(iter);
}

void RenderService::CreateDirectionalLight(RRID& id, const Vec3& direction, const Vec3& color)
{
    LD_DEBUG_ASSERT(sDirectionalLight == 0 && "currently only allows single directional light");

    sDirectionalLight = id = GUID::Get();
    sLightingUBO.DirectionalLight.Dir = { direction, 0.0f };
    sLightingUBO.DirectionalLight.Color = { color, 1.0f };
}

void RenderService::DeleteDirectionalLight(RRID id)
{
    sDirectionalLight = 0;
}

void RenderService::DrawMesh(RRID id, const Mat4& transform)
{
    LD_DEBUG_ASSERT(sHasBeginViewport);
    LD_DEBUG_ASSERT(sMeshes.find(id) != sMeshes.end());

    sDrawLists.Back().Meshes.PushBack({ id, transform });
}

void RenderService::OnViewportResize(int width, int height)
{
    mViewportWidth = width;
    mViewportHeight = height;

    // recreate swapchain
    sDevice.ResizeViewport(width, height);

    // recreate framebuffers using viewport size
    sGBuffer.Cleanup();
    sFrameBuffers.CreateGBuffer(sGBuffer, width, height);

    sGBufferGroup.Cleanup();
    sGBufferGroup.Startup(sDevice, sBindingGroups.GetGBufferBGL(), sGBuffer);
}

} // namespace LD