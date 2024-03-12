#pragma once

#include <string>
#include "Core/Header/Include/Types.h"
#include "Core/OS/Include/UID.h"
#include "Core/DSA/Include/Vector.h"
#include "Core/RenderBase/Include/RShader.h"

namespace LD {

	class RBindingGroupLayout;


	enum class RPrimitiveTopology
	{
		TriangleList = 0
	};

	enum class RAttributePollRate
	{
		PerVertex = 0,    // new vertex attributes are polled from vertex buffer slot for every vertex
		PerInstance,      // new vertex attributes are polled from vertex buffer slot for every instance
	};

	struct RVertexAttribute
	{
		u32 Location;                // location of attribute in the vertex shader
		RDataType Type;              // attribute data type in the vertex shader
		bool IsNormalized = false;
	};

	struct RVertexBufferSlot
	{
		Vector<RVertexAttribute> Attributes;
		RAttributePollRate PollRate = RAttributePollRate::PerVertex;
	};

	// the layout of attributes in vertices during pipeline input assembly
	struct RVertexLayout
	{
		Vector<RVertexBufferSlot> Slots;
	};

	struct RPipelineLayout
	{
		size_t GroupLayoutCount = 0;
		RBindingGroupLayout* GroupLayouts = nullptr;
	};

	// Info to create a graphics pipeline.
	struct RPipelineInfo
	{
		const char* Name = nullptr;
		RPrimitiveTopology PrimitiveTopology = RPrimitiveTopology::TriangleList;
		RPipelineLayout PipelineLayout;
		RVertexLayout VertexLayout;
		RShader VertexShader;
		RShader FragmentShader;
	};

	// graphics pipeline handle and interface
	class RPipeline
	{
		friend class RPipelineBase;
		friend class RPipelineGL;
	public:
		using TBase = RPipelineBase;

		inline operator bool() const { return mID != 0 && mPipeline != nullptr; }
		inline UID GetID() const { return mID; }

		inline bool operator==(const RPipeline& other) const { return mID == other.mID; }
		inline bool operator!=(const RPipeline& other) const { return mID != other.mID; }

		inline operator RPipelineBase*() const { return mPipeline; }

	private:
		UID mID = 0;
		TBase* mPipeline = nullptr;
	};

} // namespace LD