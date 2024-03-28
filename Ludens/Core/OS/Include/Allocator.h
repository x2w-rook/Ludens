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

	// Pool Allocator
	// - chunk size has to be greater than a pointer type, which is 8(4) bytes for 64(32) bit systems
	// - no relocation, unlike Vectors which need to occasionally realloc
	// - no fragmentation, as all chunks are of uniform size and pool size is fixed
	template <size_t TChunkSize>
	class PoolAllocator : public Allocator<PoolAllocator<TChunkSize>>
	{
	public:
		void Startup(int maxChunks)
		{
			mMaxChunks = maxChunks;
			mStart = MemoryAlloc(TChunkSize * mMaxChunks);
			Reset();
		}

		void Cleanup()
		{
			MemoryFree(mStart);
			mStart = nullptr;
			mMaxChunks = 0;
		}

		void Reset()
		{
			mFreeChunks.Head = nullptr;
			mFreeChunks.Size = 0;

			for (size_t i = 0; i < mMaxChunks; i++)
			{
				mFreeChunks.Push({ (Chunk*)((u8*)mStart + TChunkSize * i) });
			}
		}

		void* Alloc(size_t size)
		{
			LD_DEBUG_ASSERT(size == TChunkSize);
			LD_DEBUG_ASSERT(mFreeChunks.Size > 0);

			return (void*)mFreeChunks.Pop();
		}

		void Free(void* chunk)
		{
			LD_DEBUG_ASSERT(mFreeChunks.Size < mMaxChunks);

			mFreeChunks.Push({ (Chunk*)chunk });
		}

		inline size_t CountFreeChunks() const
		{
			return mFreeChunks.Size;
		}

		inline size_t CountUsedChunks() const
		{
			return mMaxChunks - mFreeChunks.Size;
		}

	private:
		struct Chunk
		{
			Chunk* Next;
		};

		LD_STATIC_ASSERT(TChunkSize >= sizeof(PoolAllocator::Chunk));

		struct ChunkStack
		{
			Chunk* Head = nullptr;
			size_t Size = 0;

			void Push(Chunk* chunk)
			{
				++Size;
				chunk->Next = Head;
				Head = chunk;
			}
			
			Chunk* Pop()
			{
				LD_DEBUG_ASSERT(Size > 0 && Head != nullptr);

				--Size;
				Chunk* top = Head;
				Head = Head->Next;
				return top;
			}
		};

		void* mStart = nullptr;
		size_t mMaxChunks;
		ChunkStack mFreeChunks;
	};


	class StackAllocator : public Allocator<StackAllocator>
	{
	public:

		void Startup(size_t total)
		{
			mTotal = total;
			mUsed = 0;
			mBase = MemoryAlloc(mTotal);
		}

		void Cleanup()
		{
			MemoryFree(mBase);
			mBase = nullptr;
			mTotal = 0;
			mUsed = 0;
		}

		void* Alloc(size_t size)
		{
			LD_DEBUG_ASSERT(size != 0);

			if (mUsed + size > mTotal)
				return nullptr;

			void* mem = (void*)((char*)mBase + mUsed);
			mUsed += size;
			return mem;
		}

		void Free(void* mem)
		{
			LD_DEBUG_ASSERT(mem == mBase);

			mUsed = 0;
		}

	private:

		void* mBase = nullptr;
		size_t mTotal = 0;
		size_t mUsed = 0;
	};

} // namespace LD