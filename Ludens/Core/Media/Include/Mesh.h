#pragma once

#include "Core/Math/Include/Vec2.h"
#include "Core/Math/Include/Vec3.h"
#include "Core/DSA/Include/Vector.h"

namespace LD
{

struct MeshVertex
{
    Vec3 Position;
    Vec3 Normal;
    Vec2 TexUV;

    bool operator==(const MeshVertex& other) const
    {
        const char* l = (const char*)(this);
        const char* r = (const char*)(&other);

        for (size_t i = 0; i < sizeof(MeshVertex); i++)
            if (l[i] != r[i])
                return false;

        return true;
    }
};

using MeshIndex = u32;

struct Mesh
{
    Vector<MeshVertex> Vertices;
    Vector<MeshIndex> Indices;
};

void GenerateBoxMesh(Mesh& mesh, const Vec3& halfExtent);

} // namespace LD

namespace std
{

template <>
struct hash<LD::Vec3>
{
    size_t operator()(LD::Vec3 const& vec) const
    {
        return (size_t)(vec.x * 1000000) ^ (size_t)(vec.y * 1000) ^ (size_t)(vec.z);
    }
};

template <>
struct hash<LD::Vec2>
{
    size_t operator()(LD::Vec2 const& vec) const
    {
        return (size_t)(vec.x * 1000) ^ (size_t)(vec.y);
    }
};

template <>
struct hash<LD::MeshVertex>
{
    size_t operator()(LD::MeshVertex const& vertex) const
    {
        return ((hash<LD::Vec3>()(vertex.Position) ^ (hash<LD::Vec3>()(vertex.Normal) << 1)) >> 1) ^
               (hash<LD::Vec2>()(vertex.TexUV) << 1);
    }
};

} // namespace std