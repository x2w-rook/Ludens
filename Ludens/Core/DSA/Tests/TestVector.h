#pragma once

#include <doctest.h>
#include "Core/DSA/Include/Vector.h"

using namespace LD;

TEST_CASE("Vector ctor")
{
	{
		Vector<float> v;
		CHECK(v.Size() == 0);
		CHECK(v.ByteSize() == 0);
		CHECK(v.IsEmpty());
	}

	{
		Vector<char> v(5);
		CHECK(v.Size() == 5);
		CHECK(v.ByteSize() == 5);
		CHECK(!v.IsEmpty());

		v.Front() = 1;
		v.Back() = 5;

		CHECK(v.Front() == 1);
		CHECK(v.Back() == 5);
	}
}

TEST_CASE("Vector Mutate")
{
	{
		Vector<int> v;
		for (int i = 0; i < 10; i++)
			v.PushBack(i);

		CHECK(v.Size() == 10);
		CHECK(v.ByteSize() == sizeof(int) * 10);
		CHECK(v.Front() == 0);
		CHECK(v.Back() == 9);

		for (int i = 0; i < v.Size(); i++)
		{
			CHECK(v[i] == i);
			++v[i];
			CHECK(v[i] == i + 1);
		}
	}
}

TEST_CASE("Vector Copy")
{
	{
		Vector<int> v1 = { 1, 2, 3, 4, 5 };
		Vector<int> v2(v1);

		CHECK(v2.Size() == 5);
		CHECK(v2[1] == 2);
		CHECK(v2[4] == 5);
		CHECK(v2.Front() == 1);
		CHECK(v2.Back() == 5);

		for (int i = 0; i < v2.Size(); i++)
		{
			v2[i]++;
			CHECK(v2[i] - v1[i] == 1);
		}
	}

	{
		Vector<int> v1 = { 1, 2, 3, 4 };
		Vector<int> v2 = v1;

		v2.PushBack(5);
		v2.PushBack(6);
		v2.PushBack(7);
		v2.PushBack(8);

		CHECK(v2.Back() == 8);
		CHECK(v2.Size() == 8);
		CHECK(v1.Size() == 4);

		for (int i = 0; i < v1.Size(); i++)
		{
			CHECK(v1[i] == v2[i]);
			v1[i] += 4;
			CHECK(v1[i] == v2[i + 4]);
		}
	}
}