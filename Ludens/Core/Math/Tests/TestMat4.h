#include <doctest.h>
#include "Core/Math/Include/Mat4.h"

using namespace LD;

static const Mat4 sMat4Pattern{
	{ 0.0f, 1.0f, 2.0f, 3.0f },
	{ 4.0f, 5.0f, 6.0f, 7.0f },
	{ 8.0f, 9.0f, 10.0f, 11.0f },
	{ 12.0f, 13.0f, 14.0f, 15.0f }
};

TEST_CASE("Mat4 scalar")
{
	{
		Mat4 m(sMat4Pattern);

		float scalar = 10.0f;
		m = m + scalar;

		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				CHECK(m[i][j] == sMat4Pattern[i][j] + scalar);
	}

	{
		Mat4 m(sMat4Pattern);

		float scalar = 5.0f;
		m = m - scalar;

		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				CHECK(m[i][j] == sMat4Pattern[i][j] - scalar);
	}

	{
		Mat4 m(sMat4Pattern);

		float scalar = 3.0f;
		m = m * scalar;

		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				CHECK(m[i][j] == sMat4Pattern[i][j] * scalar);
	}

	{
		Mat4 m(sMat4Pattern);

		float scalar = 2.0f;
		m = m / scalar;

		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				CHECK(m[i][j] == sMat4Pattern[i][j] / scalar);
	}
}

TEST_CASE("Mat4 Multiply")
{
	{
		Mat4 m1(sMat4Pattern);
		Vec4 v1(1.0f, 1.0f, 1.0f, 1.0f);
		Vec4 v2 = m1 * v1;

		CHECK(v2.x == 24.0f);
		CHECK(v2.y == 28.0f);
		CHECK(v2.z == 32.0f);
		CHECK(v2.w == 36.0f);
	}

	{
		Mat4 m1(sMat4Pattern);
		Mat4 m2 = Mat4::Identity;
		Mat4 m3 = m1 * m2;
		Mat4 m4 = m2 * m1;

		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
			{
				CHECK(m3[i][j] == sMat4Pattern[i][j]);
				CHECK(m4[i][j] == sMat4Pattern[i][j]);
			}
	}
}
