#pragma once

#include "Core/RenderBase/Include/RBinding.h"
#include "Core/RenderBase/Include/RTexture.h"
#include "Core/RenderBase/Include/RBuffer.h"
#include "Core/RenderFX/Include/PrefabBindingGroup.h"
#include "Core/Math/Include/Vec3.h"

namespace LD {

/// binding 0, material flat values
struct MaterialUBO
{
    alignas(16) Vec3 Ambient;
    float UseAmbientTexture;
    alignas(16) Vec3 Albedo;
    float UseAlbedoTexture;
    alignas(16) Vec3 Specular;
    float UseSpecularTexture;
    alignas(16) Vec4 UseNormalTexture;
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

    /// info to construct an ambient texture
    Optional<RTextureInfo> AmbientTextureInfo;

    /// info to construct an albedo texture
    Optional<RTextureInfo> AlbedoTextureInfo;

    /// info to construct specular texture
    Optional<RTextureInfo> SpecularTextureInfo;

    /// info to construct normal texture
    Optional<RTextureInfo> NormalTextureInfo;
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

    inline RTexture GetAmbientTexture() const
    {
        return mAmbientTexture;
    }

    inline RTexture GetAlbedoTexture() const
    {
        return mAlbedoTexture;
    }

    inline RTexture GetSpecularTexture() const
    {
        return mSpecularTexture;
    }
    
    inline RTexture GetNormalTexture() const
    {
        return mNormalTexture;
    }

private:
    RDevice mDevice;
    RBuffer mUBO;              // binding 0
    RTexture mAmbientTexture;  // binding 1
    RTexture mAlbedoTexture;   // binding 2
    RTexture mSpecularTexture; // binding 3
    RTexture mNormalTexture;   // binding 4
};

} // namespace LD