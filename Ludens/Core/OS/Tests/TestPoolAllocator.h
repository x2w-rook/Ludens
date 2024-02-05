#pragma once

#include <set>
#include <doctest.h>
#include "Core/OS/Include/Allocator.h"
#include "Core/Math/Include/Vec2.h"

using namespace LD;


TEST_CASE("PoolAllocator")
{
	int numChunks = 128;
	std::set<u8*> offsets;
	PoolAllocator<sizeof(Vec2)> pa;

	pa.Setup(numChunks);
	{
		for (int i = 0; i < numChunks; i++)
		{
			CHECK(pa.CountUsedChunks() == i);
			CHECK(pa.CountFreeChunks() == numChunks - i);
			offsets.insert((u8*)pa.Alloc(sizeof(Vec2)));
		}

		CHECK(offsets.size() == numChunks);

		int i = 0;
		u8* min = *offsets.begin();

		for (auto it = offsets.begin(); it != offsets.end(); it++, i++)
		{
			CHECK((*it - min) % sizeof(Vec2) == 0);
			CHECK((*it - min) / sizeof(Vec2) == i);

			CHECK(pa.CountFreeChunks() == i);
			CHECK(pa.CountUsedChunks() == numChunks - i);
			pa.Free(*it);
		}
	}
	pa.Cleanup();
}