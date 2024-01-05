#pragma once

#include <memory>

namespace LD {

	
	template<typename T>
	using Own = std::unique_ptr<T>;

	template<typename T, typename... TArgs>
	Own<T> MakeOwn(TArgs&&... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename... TArgs>
	Ref<T> MakeRef(TArgs&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}


} // namespace LD