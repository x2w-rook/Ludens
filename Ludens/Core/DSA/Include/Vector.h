#pragma once

#include <initializer_list>
#include "Core/Header/Include/Error.h"
#include "Core/Math/Include/Bits.h"
#include "Core/OS/Include/Memory.h"
#include "Core/OS/Include/Allocator.h"

namespace LD {

	template <typename T, typename TAllocator = MemoryAllocator>
	class Vector
	{
	public:
		Vector()
			: mSize(0)
			, mData(nullptr)
			, mAllocSize(0)
		{
		}

		Vector(size_t size)
		{
			Resize(size);
		}

		Vector(const Vector<T>& other)
		{
			Resize(other.mSize);
			std::copy(other.Begin(), other.End(), Begin());
		}

		Vector(const std::initializer_list<T>& list)
		{
			Resize(list.size());
			std::copy(list.begin(), list.end(), Begin());
		}

		~Vector()
		{
			Resize(0);
		}

		Vector<T>& operator=(const Vector<T>& other)
		{
			Resize(other.mSize);
			std::copy(other.Begin(), other.End(), Begin());
			return *this;
		}

		inline T* Data() { return mData; }
		inline const T* Data() const { return mData; }
		inline bool IsEmpty() const { return mSize == 0; }
		inline size_t Size() const { return mSize; }
		inline size_t ByteSize() const { return sizeof(T) * mSize; }

		inline void Resize(size_t size)
		{
			// deallocate
			if (size == 0)
			{
				if (mData != nullptr)
				{
					for (size_t i = 0; i < mAllocSize; i++)
						mData[i].~T();
					mAllocator.Free(mData);
					mData = nullptr;
				}
				mSize = 0;
				return;
			}

			size_t newAllocSize = NextPowerOf2((u32)size);

			// shrinking, or growing but does not trigger realloc
			if (size <= mSize || newAllocSize <= mAllocSize)
			{
				mSize = size;
				return;
			}

			// reallocate, migrate data
			T* newData = (T*)mAllocator.Alloc(sizeof(T) * newAllocSize);
			for (size_t i = 0; i < newAllocSize; i++)
				new (newData + i) T{};

			if (mData != nullptr)
			{
				for (size_t i = 0; i < mAllocSize; i++)
				{
					if (i < mSize)
						newData[i] = std::move(mData[i]);
					mData[i].~T();
				}
				mAllocator.Free(mData);
			}

			mAllocSize = newAllocSize;
			mData = newData;
			mSize = size;
		}

		inline void PushBack(const T& item)
		{
			Resize(mSize + 1);
			mData[mSize - 1] = item;
		}

		inline void PushBack(T&& item)
		{
			Resize(mSize + 1);
			mData[mSize - 1] = std::move(item);
		}

		inline void Clear() { mSize = 0; }

		inline const T* Begin() const { return mData; }
		inline const T* End() const { return mData + mSize; }
		inline T* Begin() { return mData; }
		inline T* End() { return mData + mSize; }

		// STL backwards support
		inline const T* begin() const { return Begin(); }
		inline const T* end() const { return End(); }
		inline T* begin() { return Begin(); }
		inline T* end() { return End(); }

		inline T& operator[](size_t index)
		{
			LD_DEBUG_ASSERT(index < mSize);
			return mData[index];
		}

		inline const T& operator[](size_t index) const
		{
			LD_DEBUG_ASSERT(index < mSize);
			return mData[index];
		}

	private:
		T* mData = nullptr;
		size_t mSize = 0;
		size_t mAllocSize = 0;
		TAllocator mAllocator{};
	};


} // namespace LD