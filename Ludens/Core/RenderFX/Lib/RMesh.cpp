#include "Core/RenderBase/Include/RBinding.h"
#include "Core/RenderFX/Include/RMesh.h"
#include "Core/Media/Include/Image.h"

namespace LD
{

RMesh::RMesh()
{
    mDevice.ResetHandle();
}

RMesh::~RMesh()
{
    LD_DEBUG_ASSERT(!mDevice);
}

void RMesh::Startup(const RMeshInfo& info)
{
    const Model& model = *info.Data;
    mDevice = info.Device;

    LD_DEBUG_ASSERT(mDevice);

    size_t materialCount = model.Materials.Size();
    mBatches.Resize(materialCount);

    for (size_t batchIdx = 0; batchIdx < mBatches.Size(); batchIdx++)
    {
        const Material& mat = model.Materials[batchIdx].first;
        const Vector<int>& meshRefs = model.Materials[batchIdx].second;
        Batch& batch = mBatches[batchIdx];
        MaterialGroup& matBG = batch.Material;
        MaterialGroupInfo matBGI;
        matBGI.Device = mDevice;
        matBGI.MaterialBGL = info.MaterialBGL;
        matBGI.UBO.Ambient = mat.Ambient;
        matBGI.UBO.Albedo = mat.Albedo;
        matBGI.UBO.Specular = mat.Specular;
        matBGI.UBO.UseAmbientTexture = 0;
        matBGI.UBO.UseAlbedoTexture = 0;
        matBGI.UBO.UseSpecularTexture = 0;
        matBGI.UBO.UseNormalTexture = Vec4::Zero;

        u8* albedoData = nullptr;
        u8* ambientData = nullptr;
        u8* specularData = nullptr;
        u8* normalData = nullptr;

        if (mat.AmbientTexture.HasValue())
        {
            int width, height, channels;
            LoadImage(&ambientData, mat.AmbientTexture.Value().ToString().c_str(), &width, &height, &channels);
            RTextureInfo info{};
            info.Type = RTextureType::Texture2D;
            info.Format = RTextureFormat::RGBA8;
            info.Width = (u32)width;
            info.Height = (u32)height;
            info.Data = ambientData;
            info.Size = width * height * 4;

            matBGI.AmbientTextureInfo = info;
            matBGI.UBO.UseAmbientTexture = 1.0f;
        }

        if (mat.AlbedoTexture.HasValue())
        {
            int width, height, channels;
            LoadImage(&albedoData, mat.AlbedoTexture.Value().ToString().c_str(), &width, &height, &channels);
            RTextureInfo info{};
            info.Type = RTextureType::Texture2D;
            info.Format = RTextureFormat::RGBA8;
            info.Width = (u32)width;
            info.Height = (u32)height;
            info.Data = albedoData;
            info.Size = width * height * 4;

            matBGI.AlbedoTextureInfo = info;
            matBGI.UBO.UseAlbedoTexture = 1.0f;
        }

        if (mat.SpecularTexture.HasValue())
        {
            int width, height, channels;
            LoadImage(&specularData, mat.SpecularTexture.Value().ToString().c_str(), &width, &height, &channels);
            RTextureInfo info{};
            info.Type = RTextureType::Texture2D;
            info.Format = RTextureFormat::RGBA8;
            info.Width = (u32)width;
            info.Height = (u32)height;
            info.Data = specularData;
            info.Size = width * height * 4;

            matBGI.SpecularTextureInfo = info;
            matBGI.UBO.UseSpecularTexture = 1.0f;
        }

        if (mat.NormalTexture.HasValue())
        {
            int width, height, channels;
            LoadImage(&normalData, mat.NormalTexture.Value().ToString().c_str(), &width, &height, &channels);
            RTextureInfo info{};
            info.Type = RTextureType::Texture2D;
            info.Format = RTextureFormat::RGBA8;
            info.Width = (u32)width;
            info.Height = (u32)height;
            info.Data = normalData;
            info.Size = width * height * 4;

            matBGI.NormalTextureInfo = info;
            matBGI.UBO.UseNormalTexture = { 1.0f, 1.0f, 1.0f, 1.0f };
        }

        matBG.Startup(matBGI);

        if (ambientData)
            FreeImage(ambientData);
        if (albedoData)
            FreeImage(albedoData);
        if (specularData)
            FreeImage(specularData);
        if (normalData)
            FreeImage(normalData);

        // batch all geometry that uses the current material
        Vector<RMeshVertex> batchVertices;
        Vector<u32> batchIndices;
        batch.IndexCount = 0;
        batch.VertexCount = 0;
        u32 indexBase = 0;
        u32 vertexBase = 0;

        for (int meshIdx : meshRefs)
        {
            const Mesh& mesh = model.Meshes[meshIdx].first;
            int materialRef = model.Meshes[meshIdx].second;
            LD_DEBUG_ASSERT(materialRef == batchIdx);

            vertexBase = batchVertices.Size();
            batchVertices.Resize(vertexBase + mesh.Vertices.Size());

            for (size_t vertexIdx = 0; vertexIdx < mesh.Vertices.Size(); vertexIdx++)
            {
                batchVertices[vertexBase + vertexIdx].Position = mesh.Vertices[vertexIdx].Position;
                batchVertices[vertexBase + vertexIdx].Normal = mesh.Vertices[vertexIdx].Normal;
                batchVertices[vertexBase + vertexIdx].TexUV = mesh.Vertices[vertexIdx].TexUV;
            }

            indexBase = batchIndices.Size();
            batchIndices.Resize(indexBase + mesh.Indices.Size());

            for (size_t indexIdx = 0; indexIdx < mesh.Indices.Size(); indexIdx++)
            {
                batchIndices[indexBase + indexIdx] = vertexBase + mesh.Indices[indexIdx];
            }

            batch.VertexCount += mesh.Vertices.Size();
            batch.IndexCount += mesh.Indices.Size();
        }

        RBufferInfo vboInfo{};
        vboInfo.Type = RBufferType::VertexBuffer;
        vboInfo.Data = batchVertices.Data();
        vboInfo.Size = batchVertices.ByteSize();
        mDevice.CreateBuffer(batch.Vertices, vboInfo);

        RBufferInfo iboInfo{};
        iboInfo.Type = RBufferType::IndexBuffer;
        iboInfo.Data = batchIndices.Data();
        iboInfo.Size = batchIndices.ByteSize();
        mDevice.CreateBuffer(batch.Indices, iboInfo);
    }
}

void RMesh::Cleanup()
{
    for (auto& batch : mBatches)
    {
        mDevice.DeleteBuffer(batch.Indices);
        mDevice.DeleteBuffer(batch.Vertices);
        batch.Material.Cleanup();
    }

    mDevice.ResetHandle();
}

void RMesh::Draw(BatchFn fn)
{
    LD_DEBUG_ASSERT(mDevice);

    for (auto& batch : mBatches)
    {
        fn(batch);
    }
}

} // namespace LD