#include "Core/Media/Include/Mesh.h"

namespace LD
{

void GenerateBoxMesh(Mesh& mesh, const Vec3& halfExtent)
{
    mesh.Vertices.Clear();
    mesh.Indices.Clear();

    int idxBase = 0;
    MeshVertex p0, p1, p2, p3;
    p0.TexUV = { 0.0f, 0.0f };
    p1.TexUV = { 0.0f, 0.0f };
    p2.TexUV = { 0.0f, 0.0f };
    p3.TexUV = { 0.0f, 0.0f };

    for (int sign = -1; sign <= 1; sign += 2)
    {
        // clang-format off

        p0.Position = { sign * -halfExtent.x, sign * halfExtent.y, -halfExtent.z };
        p1.Position = { sign * -halfExtent.x, sign * halfExtent.y,  halfExtent.z };
        p2.Position = { sign *  halfExtent.x, sign * halfExtent.y,  halfExtent.z };
        p3.Position = { sign *  halfExtent.x, sign * halfExtent.y, -halfExtent.z };
        p0.Normal = { 0.0f, (float)sign, 0.0f };
        p1.Normal = { 0.0f, (float)sign, 0.0f };
        p2.Normal = { 0.0f, (float)sign, 0.0f };
        p3.Normal = { 0.0f, (float)sign, 0.0f };

        mesh.Vertices.PushBack(p0);
        mesh.Vertices.PushBack(p1);
        mesh.Vertices.PushBack(p2);
        mesh.Vertices.PushBack(p3);

        mesh.Indices.PushBack(idxBase + 0);
        mesh.Indices.PushBack(idxBase + 1);
        mesh.Indices.PushBack(idxBase + 2);
        mesh.Indices.PushBack(idxBase + 2);
        mesh.Indices.PushBack(idxBase + 3);
        mesh.Indices.PushBack(idxBase + 0);
        idxBase += 4;

        p0.Position = { sign * halfExtent.x, sign *  halfExtent.y, halfExtent.z };
        p1.Position = { sign * halfExtent.x, sign * -halfExtent.y, halfExtent.z };
        p2.Position = { sign * halfExtent.x, sign * -halfExtent.y, -halfExtent.z };
        p3.Position = { sign * halfExtent.x, sign *  halfExtent.y, -halfExtent.z };
        p0.Normal = { (float)sign, 0.0f, 0.0f };
        p1.Normal = { (float)sign, 0.0f, 0.0f };
        p2.Normal = { (float)sign, 0.0f, 0.0f };
        p3.Normal = { (float)sign, 0.0f, 0.0f };

        mesh.Vertices.PushBack(p0);
        mesh.Vertices.PushBack(p1);
        mesh.Vertices.PushBack(p2);
        mesh.Vertices.PushBack(p3);

        mesh.Indices.PushBack(idxBase + 0);
        mesh.Indices.PushBack(idxBase + 1);
        mesh.Indices.PushBack(idxBase + 2);
        mesh.Indices.PushBack(idxBase + 2);
        mesh.Indices.PushBack(idxBase + 3);
        mesh.Indices.PushBack(idxBase + 0);
        idxBase += 4;

        p0.Position = { -halfExtent.x, sign *  halfExtent.y, sign * halfExtent.z };
        p1.Position = { -halfExtent.x, sign * -halfExtent.y, sign * halfExtent.z };
        p2.Position = {  halfExtent.x, sign * -halfExtent.y, sign * halfExtent.z };
        p3.Position = {  halfExtent.x, sign *  halfExtent.y, sign * halfExtent.z };
        p0.Normal = { 0.0f, 0.0f, (float)sign };
        p1.Normal = { 0.0f, 0.0f, (float)sign };
        p2.Normal = { 0.0f, 0.0f, (float)sign };
        p3.Normal = { 0.0f, 0.0f, (float)sign };

        mesh.Vertices.PushBack(p0);
        mesh.Vertices.PushBack(p1);
        mesh.Vertices.PushBack(p2);
        mesh.Vertices.PushBack(p3);

        mesh.Indices.PushBack(idxBase + 0);
        mesh.Indices.PushBack(idxBase + 1);
        mesh.Indices.PushBack(idxBase + 2);
        mesh.Indices.PushBack(idxBase + 2);
        mesh.Indices.PushBack(idxBase + 3);
        mesh.Indices.PushBack(idxBase + 0);
        idxBase += 4;
        
        // clang-format on
    }
}

} // namespace LD