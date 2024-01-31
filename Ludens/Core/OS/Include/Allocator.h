#pragma once

#include "Core/Header/Include/Error.h"
#include "Core/OS/Include/Memory.h"


namespace LD {

	template <typename TDerived>
	class Allocator
	{
	public:
		void* Alloc(size_t size) { return static_cast<TDerived*>(this)->Alloc(size); }
		void Free(void* data) { static_cast<TDerived*>(this)->Free(data); }
	};

	// default memory allocator
	class MemoryAllocator : public Allocator<MemoryAllocator>
	{
	public:
		void* Alloc(size_t size)
		{
			return MemoryAlloc(size);
		}

		void Free(void* data)
		{
			MemoryFree(data);
		}
	};

} // namespace LD