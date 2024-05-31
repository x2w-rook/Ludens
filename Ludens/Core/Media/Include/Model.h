#pragma once

#include <utility>
#include <string>
#include "Core/Header/Include/Types.h"
#include "Core/Math/Include/Hex.h"
#include "Core/Math/Include/Vec3.h"
#include "Core/DSA/Include/Vector.h"
#include "Core/DSA/Include/Optional.h"
#include "Core/IO/Include/FileSystem.h"
#include "Core/Media/Include/Mesh.h"

namespace LD
{

/// Material plain-old-data, non-programmable
struct Material
{
    Vec3 Ambient;
    Vec3 Albedo;
    Vec3 Specular;
    Optional<Path> AmbientTexture;
    Optional<Path> AlbedoTexture;
    Optional<Path> SpecularTexture;

    static Material GetDefault()
    {
        Vec3 pearlWhite = Hex(0xF8F6F0FF).RGB();

        Material mtl;
        mtl.Albedo = pearlWhite;
        mtl.Ambient = pearlWhite;
        mtl.Specular = { 0.5f, 0.5f, 0.5f };
        mtl.AlbedoTexture.Reset();
        mtl.AmbientTexture.Reset();
        mtl.SpecularTexture.Reset();

        return mtl;
    }
};

/// plain-old-data for a Model, will be further processed by the renderer or physics engine
struct Model
{
    using MaterialRef = int;
    using MeshRefs = Vector<int>;

    /// each Mesh references a single material
    Vector<std::pair<Mesh, MaterialRef>> Meshes;

    /// each Material is used by one or more meshes
    Vector<std::pair<Material, MeshRefs>> Materials;
};

class ModelLoader
{
public:
    ModelLoader();
    ModelLoader(const ModelLoader&) = delete;
    ~ModelLoader();

    ModelLoader& operator=(const ModelLoader&) = delete;

    void LoadModel(const Path& path, Model& model);

private:
};

} // namespace LD