#pragma once


namespace LD {

	enum class RBackend
	{
		OpenGL = 0,
		Vulkan,
	};

	enum class RMemoryUsage
	{
		// memory that is initialized and uploaded once to GPU during resource creation.
		// note that the memory is only immutable from the CPU side, GPU can still write.
		Immutable = 0,

		// memory that is expected to be written by CPU every frame.
		FrameDynamic,
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