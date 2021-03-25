
#include <gtest/gtest.h>

#include <maths/circle.h>
#include <maths/sphere.h>
#include <corecrt_math_defines.h>


TEST(Maths, Circle_CalculateArea)
{
	maths::Circle c{ 1.0f,maths::Vector2f{1.0f,2.0f} };
	ASSERT_FLOAT_EQ(c.area(), M_PI * (c.radius() * c.radius()));
}

TEST(Maths, Sphere_CalculateArea)
{
	maths::Sphere s{ 1.0f,maths::Vector3f{1.0f,2.0f,1.0f} };
	ASSERT_FLOAT_EQ(s.area(), 4 * M_PI * (s.radius() * s.radius()));
}

TEST(Maths, Sphere_CalculateVolume)
{
	maths::Sphere s{ 1.0f,maths::Vector3f{1.0f,2.0f,1.0f} };
	ASSERT_FLOAT_EQ(s.volume(), 4 / 3 * M_PI * (s.radius() * s.radius()* s.radius()));
}