#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Core/Core.h>
#include "Core/Math/Include/Vec3.h"

// Type Conversions
// - Jolt is by default compiled with 32-bit floating point precision
// - Jolt matrices are column major

namespace LD
{

inline JPH::Vec3 ToJolt(const Vec3& v)
{
    return JPH::Vec3(v.x, v.y, v.z);
}

inline Vec3& FromJolt(const JPH::Vec3& v)
{
    return LD::Vec3(v.GetX(), v.GetY(), v.GetZ());
}

} // namespace LD