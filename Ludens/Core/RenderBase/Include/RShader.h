#pragma once

#include "Core/Header/Include/Types.h"
#include "Core/OS/Include/UID.h"

namespace LD {

	enum class RShaderType
	{
		VertexShader = 0,
		FragmentShader
	};

	enum class RDataType
	{
		Float,
		Vec2,
		Vec3,
		Vec4,
	};

	struct RShaderInfo
	{
		RShaderType Type;
		const void* Data = nullptr;
		u32 Size = 0;
	};

	// shader handle and interface
	class RShader
	{
		friend class RShaderBase;
		friend class RShaderGL;
	public:
		using TBase = RShaderBase;

		inline operator RShaderBase*() const { return mShader; }

	private:
		UID mID = 0;
		RShaderBase* mShader = nullptr;
	};

} // namespace LD