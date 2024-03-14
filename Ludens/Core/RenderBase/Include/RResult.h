#pragma once

namespace LD {

	enum class RBufferType
	{
		VertexBuffer = 0,
		IndexBuffer,
		UniformBuffer,
	};

	enum class RShaderType
	{
		VertexShader = 0,
		FragmentShader
	};

	enum class RResultType
	{
		Ok = 0,
		InvalidHandle,
		InvalidIndex,
		ResourceMissing,
		BufferTypeMismatch,
		ShaderTypeMismatch,
		BindingGroupMismatch,
		BindingMismatch,
	};

	enum class RResourceType
	{
		Device = 0,
		Texture,
		Buffer,
		Shader,
		FrameBuffer,
		BindingGroupLayout,
		BindingGroup,
		Pipeline,
	};

	struct RResourceMissing
	{
		RResourceType MissingType;
	};

	struct RBufferTypeMismatch
	{
		RBufferType Expect;
		RBufferType Actual;
	};

	struct RShaderTypeMismatch
	{
		RShaderType Expect;
		RShaderType Actual;
	};

	struct RResult
	{
		RResultType Type = RResultType::Ok;

		operator bool() const { return Type == RResultType::Ok; }
		
		union
		{
			RResourceMissing ResourceMissing;
			RBufferTypeMismatch BufferTypeMismatch;
			RShaderTypeMismatch ShaderTypeMismatch;
		};
	};

	const char* RResourceTypeString(RResourceType type);

} // namespace LD