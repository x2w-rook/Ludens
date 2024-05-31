#pragma once

#include "Core/Header/Include/Singleton.h"
#include "Core/OS/Include/UID.h"
#include "Core/RenderBase/Include/RTypes.h"

namespace LD
{

class UIContext;

/// renderer resource id
using RRID = UID;

class RenderService : public Singleton<RenderService>
{
    friend class Singleton<RenderService>;

public:
    void Startup(RBackend backend);
    void Cleanup();

    void BeginFrame();
    void EndFrame();

    void BeginViewport(const Vec3& viewpos, const Mat4& view, const Mat4& projection);
    void EndViewport();

    void CreateMesh(RRID& id, const Model* model);
    void DeleteMesh(RRID id);

    void CreateDirectionalLight(RRID& id, const Vec3& direction, const Vec3& color);
    void DeleteDirectionalLight(RRID id);

    void DrawMesh(RRID mesh, const Mat4& transform);
};

} // namespace LD