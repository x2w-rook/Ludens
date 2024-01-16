#pragma once

#include <memory>

namespace LD {
	
	///
	/// HEAP MEMORY
	///
	
	void* MemoryAlloc(size_t size);
	void* MemoryRealloc(void* mem, size_t size);
	void MemoryFree(void* mem);

	///
	/// SMART POINTERS
	///

	template<typename T>
	using Own = std::unique_ptr<T>;

	template<typename T, typename... TArgs>
	Own<T> MakeOwn(TArgs&&... args)
	{
		return std::make_unique<T>(std::forward<TArgs>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename... TArgs>
	Ref<T> MakeRef(TArgs&&... args)
	{
		return std::make_shared<T>(std::forward<TArgs>(args)...);
	}

} // namespace LD
