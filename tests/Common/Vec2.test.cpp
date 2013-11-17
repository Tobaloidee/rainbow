// Copyright (c) 2010-14 Bifrost Entertainment AS and Tommy Nguyen
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)

#include "Common/Vec2.h"

TEST(Vec2Test, Angle)
{
	Vec2i v0;
	Vec2i v1(1, 1);
	ASSERT_FLOAT_EQ(0.7853982f, v0.angle(v1));
}

TEST(Vec2Test, Distance)
{
	Vec2i v0;
	Vec2i v1(3, 4);
	ASSERT_FLOAT_EQ(5.0f, v0.distance(v1));
}

TEST(Vec2Test, DotProduct)
{
	Vec2i v0(3, 4);
	ASSERT_FLOAT_EQ(25.0f, v0.dot(v0));
}

TEST(Vec2Test, OperatorAdd)
{
	Vec2i v0(1, 2);
	Vec2i v1(3, 4);
	v0 += v1;
	ASSERT_EQ(4, v0.x);
	ASSERT_EQ(6, v0.y);
}

TEST(Vec2Test, OperatorEq)
{
	Vec2i v0;
	Vec2i v1(5, 2);
	v0 = v1;
	ASSERT_EQ(5, v0.x);
	ASSERT_EQ(2, v0.y);
}

TEST(Vec2Test, OperatorMul)
{
	Vec2i v0(5, 2);
	v0 *= 2;
	ASSERT_EQ(10, v0.x);
	ASSERT_EQ(4, v0.y);
}

TEST(Vec2Test, OperatorSub)
{
	Vec2i v0(1, 2);
	Vec2i v1(3, 4);
	v0 -= v1;
	ASSERT_EQ(-2, v0.x);
	ASSERT_EQ(-2, v0.y);
}

TEST(Vec2Test, Zero)
{
	Vec2i v0;
	ASSERT_EQ(0, v0.x);
	ASSERT_EQ(0, v0.y);

	v0.x = 1;
	v0.y = 1;
	ASSERT_FALSE(v0.is_zero());
}
