#pragma once

#include <cstddef>
#include "Core/Header/Include/Error.h"

namespace LD {

	template <typename T>
	class View
	{
	public:
		View()
			: mData(nullptr)
			, mSize(0)
		{}
		View(size_t size, const T* data)
			: mData(data)
			, mSize(size)
		{}

		operator const T*() const { return mData; }
		inline const T* Data() const { return mData; }
		inline size_t Size() const { return mSize; }

		const T& operator[](int idx) const
		{
			LD_DEBUG_ASSERT(0 <= idx && idx < (int)mSize);
			return mData[idx];
		}

		// STL backwards support
		inline const T* begin() const { return Begin(); }
		inline const T* end() const { return End(); }
		inline const T* Begin() const { return mData; }
		inline const T* End() const { return mData + mSize; }

	private:
		const T* mData;
		size_t mSize;
	};


} // namespace LD
