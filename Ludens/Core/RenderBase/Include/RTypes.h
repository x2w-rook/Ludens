#pragma once


namespace LD {

	enum class RBackend
	{
		OpenGL = 0,
		Vulkan,     // not implemented yet, but shaders are written in Vulkan GLSL
	};

	enum class RMemoryUsage
	{
		Immutable = 0,
	};

	struct RDrawVertexInfo
	{
		u32 VertexCount = 0;
		u32 VertexStart = 0;
		u32 InstanceCount = 1;
		u32 InstanceStart = 0;
	};

	enum class RIndexType
	{
		u16 = 0,
		u32
	};

	struct RDrawIndexedInfo
	{
		u32 IndexCount = 0;
		u32 IndexStart = 0;
		u32 InstanceCount = 1;
		u32 InstanceStart = 0;
	};

} // namespace LD