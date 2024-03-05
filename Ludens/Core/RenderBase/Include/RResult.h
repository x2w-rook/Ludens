#pragma once

namespace LD {

	enum class RResultType
	{
		Ok = 0,
		ResourceMissing,
		BufferMismatch,
		ShaderMismatch,
		BindingGroupMismatch,
		BindingMismatch,
	};

	struct RResult
	{
		RResultType Type = RResultType::Ok;

		// SPACE: for union of all error structs
	};

} // namespace LD