#pragma once

#include "Core/OS/Include/UID.h"
#include "Core/RenderBase/Include/RShader.h"
#include "Core/RenderBase/Include/RResult.h"

namespace LD {

	enum class RBufferType
	{
		VertexBuffer = 0,
		IndexBuffer,
		UniformBuffer,
	};

	struct RBufferInfo
	{
		RShaderStageFlags BindStages;
		RBufferType Type;
		const void* Data = nullptr;
		u32 Size = 0;
	};

	// render buffer handle and interface
	class RBuffer
	{
		friend class RBufferBase;
		friend class RBufferGL;
	public:
		using TBase = RBufferBase;

		RResult SetData(u32 offset, u32 size, const void* data);

		inline bool IsValid() const { return mID != 0 && mBuffer != nullptr; }
		inline UID GetID() const { return mID; }

		inline bool operator==(const RBuffer& other) const { return mID == other.mID; }
		inline bool operator!=(const RBuffer& other) const { return mID != other.mID; }

		inline operator RBufferBase*() const { return mBuffer; }

	private:
		UID mID = 0;
		RBufferBase* mBuffer = nullptr;
	};

} // namespace LD