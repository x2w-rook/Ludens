#pragma once

#include <initializer_list>
#include "Core/Header/Include/Error.h"
#include "Core/OS/Include/Memory.h"

namespace LD {

	template <typename T>
	class Vector
	{
	public:
		Vector() = default;
		Vector(size_t size);
		Vector(const Vector<T>& other);
		Vector(const std::initializer_list<T>& list);
		~Vector();

		Vector<T>& operator=(const Vector<T>& other);

		inline T* Data() { return mData; }
		inline const T* Data() const { return mData; }
		inline bool IsEmpty() const { return mSize == 0; }
		inline size_t Size() const { return mSize; }
		inline size_t ByteSize() const { return sizeof(T) * mSize; }

		inline void Resize(size_t size);
		inline void PushBack(const T&);

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
	};

	template <typename T>
	Vector<T>::Vector(size_t size)
	{
		mSize = size;
		mData = (T*)MemoryAlloc(sizeof(T) * size);
	}

	template <typename T>
	Vector<T>::Vector(const Vector<T>& other)
	{
		Resize(other.mSize);
		std::copy(other.Begin(), other.End(), Begin());
	}

	template <typename T>
	Vector<T>::Vector(const std::initializer_list<T>& list)
	{
		Resize(list.size());
		std::copy(list.begin(), list.end(), Begin());
	}

	template <typename T>
	Vector<T>::~Vector()
	{
		if (mData != nullptr)
			MemoryFree(mData);
	}

	template <typename T>
	Vector<T>& Vector<T>::operator=(const Vector<T>& other)
	{
		Resize(other.mSize);
		std::copy(other.Begin(), other.End(), Begin());
		return *this;
	}

	template <typename T>
	void Vector<T>::Resize(size_t size)
	{
		if (size <= mSize)
		{
			mSize = size;
			return;
		}

		mData = (T*)MemoryRealloc(mData, sizeof(T) * size);
		mSize = size;
	}

	template <typename T>
	void Vector<T>::PushBack(const T& item)
	{
		Resize(mSize + 1);
		mData[mSize - 1] = item;
	}

} // namespace LD