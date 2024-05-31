#include <iostream>
#include <unordered_map>
#include <tiny_obj_loader.h>
#include "Core/Math/Include/Hex.h"
#include "Core/OS/Include/Time.h"
#include "Core/IO/Include/FileSystem.h"
#include "Core/Media/Include/Model.h"

namespace LD
{

struct TinyObjContext
{
    Model* Target = nullptr;
    std::string DirectoryPath;
    std::string FilePath;
    tinyobj::attrib_t Attrib;
    std::vector<tinyobj::shape_t> Shapes;
    std::vector<tinyobj::material_t> Materials;
    std::unordered_map<int, int> MaterialRefMap;
    std::unordered_map<MeshVertex, MeshIndex> UniqueVertices{};
    int FallbackMaterialIdx;
};

static void TinyObjParseModel(TinyObjContext& obj);
static void TinyObjParseShape(TinyObjContext& obj, int obj_shape_idx);
static void TinyObjParseMtl(TinyObjContext& obj, int obj_shape_idx, int obj_mat_id);
static void TinyObjFallbackMtl(TinyObjContext& obj, int obj_shape_idx);

ModelLoader::ModelLoader()
{
}

ModelLoader::~ModelLoader()
{
}

void ModelLoader::LoadModel(const Path& path, Model& model)
{
    // NOTE: Temporary Solution, using tinyobj to parse .obj models into some engine standard format,
    //       will support more model formats later, for now we just want to draw some models.

    const auto& fs_path = static_cast<const std::filesystem::path&>(path);

    TinyObjContext obj{};
    obj.Target = &model;
    obj.DirectoryPath = fs_path.parent_path().string() + "/";
    obj.FilePath = fs_path.string();
    obj.FallbackMaterialIdx = -1;

    Timer timer{};
    timer.Start();

    TinyObjParseModel(obj);

    timer.Stop();
    double loadTime = timer.GetMilliSeconds();

    printf("=== ModelLoader::LoadModel ===\n");
    printf("path: %s\n", obj.FilePath.c_str());
    printf("meshes: %d\n", (int)model.Meshes.Size());
    printf("vertices: %d\n", (int)obj.UniqueVertices.size());
    printf("materials: %d\n", (int)model.Materials.Size());
    printf("load time:  %.3f ms\n", loadTime);
}

static void TinyObjParseModel(TinyObjContext& obj)
{
    std::string warn, err;

    bool triangulate = true;
    bool success = tinyobj::LoadObj(&obj.Attrib, &obj.Shapes, &obj.Materials, &warn, &err, obj.FilePath.c_str(),
                                    obj.DirectoryPath.c_str(), triangulate);

    if (!warn.empty())
        std::cout << warn << std::endl;

    if (!err.empty())
        std::cout << err << std::endl;

    LD_DEBUG_ASSERT(success);

    Model& model = *obj.Target;
    model.Materials.Clear();
    model.Meshes.Resize(obj.Shapes.size());

    for (int shapeIdx = 0; shapeIdx < (int)obj.Shapes.size(); shapeIdx++)
    {
        const tinyobj::shape_t& shape = obj.Shapes[shapeIdx];
        LD_DEBUG_ASSERT(shape.mesh.material_ids.size() * 3 == shape.mesh.indices.size() && "shape is not triangulated");

        TinyObjParseShape(obj, shapeIdx);
    }
}

// convert each tinyobj::shape_t to one Model::Mesh
// - each Model::Mesh at most references one Model::Material
// - this assumes tinyobj::shape_t only uses single tinyobj::material_t
static void TinyObjParseShape(TinyObjContext& obj, int obj_shape_idx)
{
    const tinyobj::shape_t& obj_shape = obj.Shapes[obj_shape_idx];
    auto& mesh = obj.Target->Meshes[obj_shape_idx];
    Mesh& ld_mesh = mesh.first;
    ld_mesh.Indices.Clear();
    ld_mesh.Vertices.Clear();

    LD_DEBUG_ASSERT(obj_shape.mesh.material_ids.size() > 0);
    int obj_mat_id = obj_shape.mesh.material_ids[0];
    for (int id : obj_shape.mesh.material_ids)
        LD_DEBUG_ASSERT(id == obj_mat_id);

    TinyObjParseMtl(obj, obj_shape_idx, obj_mat_id);

    for (size_t idx = 0; idx < obj_shape.mesh.indices.size(); idx++)
    {
        tinyobj::index_t index = obj_shape.mesh.indices[idx];

        MeshVertex vertex;
        vertex.Normal = Vec3::Zero;
        vertex.TexUV = Vec2::Zero;

        vertex.Position = {
            obj.Attrib.vertices[3 * index.vertex_index + 0],
            obj.Attrib.vertices[3 * index.vertex_index + 1],
            obj.Attrib.vertices[3 * index.vertex_index + 2],
        };

        if (index.normal_index >= 0)
        {
            vertex.Normal = {
                obj.Attrib.normals[3 * index.normal_index + 0],
                obj.Attrib.normals[3 * index.normal_index + 1],
                obj.Attrib.normals[3 * index.normal_index + 2],
            };
        }

        if (index.texcoord_index >= 0)
        {
            vertex.TexUV = {
                obj.Attrib.texcoords[2 * index.texcoord_index + 0],
                1.0f - obj.Attrib.texcoords[2 * index.texcoord_index + 1],
            };
        }

        if (obj.UniqueVertices.count(vertex) == 0)
        {
            obj.UniqueVertices[vertex] = (MeshIndex)(ld_mesh.Vertices.Size());
            ld_mesh.Vertices.PushBack(vertex);
        }

        ld_mesh.Indices.PushBack(obj.UniqueVertices[vertex]);
    }
}

// convert each tinyobj::material_t to Model::Material
static void TinyObjParseMtl(TinyObjContext& obj, int obj_shape_idx, int obj_mat_id)
{
    auto& mesh = obj.Target->Meshes[obj_shape_idx];
    int& materialRef = mesh.second;
    Mesh& ld_mesh = mesh.first;
    int ld_mat_idx;

    // the mesh does not reference any material, use the fallback material
    if (obj_mat_id < 0)
    {
        TinyObjFallbackMtl(obj, obj_shape_idx);
        return;
    }

    if (obj.MaterialRefMap.find(obj_mat_id) != obj.MaterialRefMap.end())
    {
        ld_mat_idx = obj.MaterialRefMap[obj_mat_id];
        materialRef = ld_mat_idx;
        obj.Target->Materials[ld_mat_idx].second.PushBack(obj_shape_idx);
        return;
    }

    // import new material
    tinyobj::material_t& obj_mat = obj.Materials[obj_mat_id];
    auto& ld_mats = obj.Target->Materials;
    ld_mat_idx = ld_mats.Size();
    
    ld_mats.PushBack({});
    Material& ld_mat = ld_mats.Back().first;
    Vector<int>& meshRefs = ld_mats.Back().second;
    
    meshRefs = { (int)obj_shape_idx };

    materialRef = ld_mat_idx;
    obj.MaterialRefMap[obj_mat_id] = ld_mat_idx;

    ld_mat.Ambient = { obj_mat.ambient[0], obj_mat.ambient[1], obj_mat.ambient[2] };
    ld_mat.Albedo = { obj_mat.diffuse[0], obj_mat.diffuse[1], obj_mat.diffuse[2] };
    ld_mat.Specular = { obj_mat.specular[0], obj_mat.specular[1], obj_mat.specular[2] };
    ld_mat.AmbientTexture.Reset();
    ld_mat.AlbedoTexture.Reset();
    ld_mat.SpecularTexture.Reset();

    if (!obj_mat.ambient_texname.empty())
        ld_mat.AmbientTexture = Path{ obj.DirectoryPath + obj_mat.ambient_texname };

    if (!obj_mat.diffuse_texname.empty())
        ld_mat.AlbedoTexture = Path{ obj.DirectoryPath + obj_mat.diffuse_texname };

    if (!obj_mat.specular_texname.empty())
        ld_mat.SpecularTexture = Path{ obj.DirectoryPath + obj_mat.specular_texname };
}

void TinyObjFallbackMtl(TinyObjContext& obj, int obj_shape_idx)
{
    auto& mesh = obj.Target->Meshes[obj_shape_idx];
    int& materialRef = mesh.second;
    Mesh& ld_mesh = mesh.first;

    if (obj.FallbackMaterialIdx < 0)
    {
        obj.FallbackMaterialIdx = (int)obj.Target->Materials.Size();
        obj.Target->Materials.PushBack({});

        Material& fallback = obj.Target->Materials.Back().first;
        fallback = Material::GetDefault();

        Vector<int>& meshRefs = obj.Target->Materials.Back().second;
        meshRefs.Clear();
    }

    materialRef = obj.FallbackMaterialIdx;
    Vector<int>& meshRefs = obj.Target->Materials[materialRef].second;
    meshRefs.PushBack(obj_shape_idx);
}

} // namespace LD