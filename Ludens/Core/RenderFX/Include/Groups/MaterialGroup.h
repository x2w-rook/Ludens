#pragma once

#include "Core/Header/Include/Types.h"
#include "Core/RenderBase/Include/RBinding.h"
#include "Core/RenderBase/Include/RTexture.h"
#include "Core/RenderBase/Include/RBuffer.h"
#include "Core/RenderFX/Include/PrefabBindingGroup.h"
#include "Core/Math/Include/Vec3.h"

namespace LD {

/// binding 0, material flat values
struct MaterialUBO
{
    i32 UseAlbedoTexture;
    i32 UseNormalTexture;
    f32 Roughness;
    f32 Metallic;
    alignas(16) Vec4 Albedo;
};

LD_STATIC_ASSERT(sizeof(MaterialUBO) % 16 == 0);

struct MaterialGroupInfo
{
    /// the owning device of the binding group and subsequent resources
    RDevice Device;

    /// a layout compatible with the material binding group
    RBindingGroupLayout MaterialBGL;

    /// describe flat colors or specify textures for the material
    MaterialUBO UBO;

    /// info to construct albedo texture
    Optional<RTextureInfo> AlbedoTextureInfo;

    /// info to construct normal texture
    Optional<RTextureInfo> NormalTextureInfo;

    /// info to construct metallic texture
    Optional<RTextureInfo> MetallicTextureInfo;

    /// info to construct roughness texture
    Optional<RTextureInfo> RoughnessTextureInfo;
};

/// non-programmable, plain-old-data material
class MaterialGroup : public PrefabBindingGroup
{
public:
    MaterialGroup() = default;
    MaterialGroup(const MaterialGroup&) = delete;
    ~MaterialGroup();

    MaterialGroup& operator=(const MaterialGroup&) = delete;

    /// startup the material binding group
    void Startup(const MaterialGroupInfo& info);

    /// cleanup the material binding group, the BGL during Startup() will not be deleted
    void Cleanup();

    virtual RBindingGroupLayoutData GetLayoutData() const override;

    virtual RBindingGroupLayout CreateLayout(RDevice device) override;

    inline RTexture GetAlbedoTexture() const
    {
        return mAlbedoTexture;
    }

    inline RTexture GetNormalTexture() const
    {
        return mNormalTexture;
    }
    
    inline RTexture GetMetallicTexture() const
    {
        return mAlbedoTexture;
    }

    inline RTexture GetRoughnessTexture() const
    {
        return mNormalTexture;
    }

private:
    RDevice mDevice;
    RBuffer mUBO;               // binding 0
    RTexture mAlbedoTexture;    // binding 1
    RTexture mNormalTexture;    // binding 2
    RTexture mMetallicTexture;  // binding 3
    RTexture mRoughnessTexture; // binding 4
};

} // namespace LD