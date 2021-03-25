/*
MIT License

Copyright (c) 2021 SAE Institute Geneva

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <gtest/gtest.h>

#include "maths/ray2.h"
#include "maths/ray3.h"

namespace maths {
 
TEST(Maths, Ray_PointInRay)
{
	// Test PointInRay Ray2
	Vector2f a{ 1.0f,2.0f };
	Vector2f b{ 2.0f,3.0f };
	Ray2 ray{ a,b };
	
	float v = 2.0f;
	
	Vector2f d = a + (b * v);
	EXPECT_EQ(ray.PointInRay(v), d);

	// Test PointInRay Ray3
	Vector3f g{ 1.0f,2.0f,1.0f };
	Vector3f h{ 2.0f,3.0f,1.0f };
	Ray3 ray3{ g,h };

	float v2 = 2.0f;

	const Vector3f i = g + (h * v2);
	const Vector3f k = ray3.PointInRay(v2);
	EXPECT_EQ( k.x, i.x);
	EXPECT_EQ(k.y, i.y);
	EXPECT_EQ(k.z, i.z);
}


	
TEST(Maths, Ray_IntersectAABB2)
{
	Vector2f a{ -0.5f,-0.5f };
	Vector2f b{ 0.5f,0.5f };
	AABB2 aabb{ a,b };

	Vector2f origin{ -0.1f,-0.1f };
	Vector2f direction{ 1.0f,1.0f };
	Ray2 ray{ origin,direction };

	// inside intersection
	ASSERT_TRUE(ray.IntersectAABB2(aabb));

	// basic intersection
	origin = Vector2f(-1.0f, -1.0f);
	direction = Vector2f(1.0f, 1.0f);
	ray = Ray2 { origin,direction };
	ASSERT_TRUE(ray.IntersectAABB2(aabb));
	
	// limit intersection
	origin = Vector2f(-0.5f, -1.5f);
	ray = Ray2 { origin,direction };
	ASSERT_TRUE(ray.IntersectAABB2(aabb));

	// no intersection
	origin = Vector2f(-0.5f, -2.0f);
	ray = Ray2 { origin,direction };
	ASSERT_FALSE(ray.IntersectAABB2(aabb));

	
}

TEST(Maths, Ray_interceptABB3)
{
	Vector3f a{ -0.5f,-0.5f,-0.5f };
	Vector3f b{ 0.5f,0.5f,0.5f };
	AABB3 aabb{ a,b };

	Vector3f origin { -0.1f,-0.1f,-0.1f };
	Vector3f direction{ 1.0f,1.0f,1.0f };
	Ray3 ray{ origin,direction};
	// inside intersection
	ASSERT_TRUE(ray.IntersectAABB3(aabb));
	
	origin = Vector3f{ -1.0f,-1.0f,-1.0f };
	ray = Ray3{ origin, direction };
	// basic intersection
	ASSERT_TRUE(ray.IntersectAABB3(aabb));

	origin = Vector3f{ -0.5f,-1.5f,-1.5f };
	ray = Ray3{ origin, direction };
	// limit intersection
	ASSERT_TRUE(ray.IntersectAABB3(aabb));

	origin = Vector3f{ -0.5f, -2.0f, -2.0f };
	ray = Ray3{ origin, direction };
	// no intersection
	ASSERT_FALSE(ray.IntersectAABB3(aabb));
}

TEST(Maths, Ray_intersectCircle)
{
	Vector2f center{ 0.0f,0.0f };
	float radius = 1.0f;
	Circle circle{ radius,center };

	Vector2f origin{ 0.0f,0.0f };
	Vector2f direction{ 1.0f,1.0f };
	Ray2 ray{ origin,direction };
	// inside intersection
	ASSERT_TRUE(ray.IntersectCircle(circle));
	
	origin = Vector2f{ -1.0f,-1.0f };
	ray = Ray2{ origin,direction };
	// basic intersection
	ASSERT_TRUE(ray.IntersectCircle(circle));

	origin = Vector2f{ -1.0f,-2.0f };
	ray = Ray2{ origin,direction };
	// limit intersection
	ASSERT_TRUE(ray.IntersectCircle(circle));

	origin = Vector2f{ -1.0f,-2.5f };
	ray = Ray2{ origin,direction };
	// no intersection
	ASSERT_FALSE(ray.IntersectCircle(circle));
}

TEST(Maths, Ray_intersectSphere)
{
	Vector3f center{ 0.0f,0.0f,0.0f };
	float radius = 1.0f;
	Sphere sphere{ radius,center };

	Vector3f origin{ 0.0f,0.0f,0.0f };
	Vector3f direction{ 1.0f,1.0f,1.0f };
	Ray3 ray{ origin,direction };
	// inside intersection
	//ASSERT_TRUE(ray.IntersectSphere(sphere));

	origin = Vector3f{ -1.0f,-1.0f,-1.0f };
	ray = Ray3{ origin,direction };
	// basic intersection
	//ASSERT_TRUE(ray.IntersectSphere(sphere));

	origin = Vector3f{ -1.0f,-1.0f,-2.0f };
	ray = Ray3{ origin,direction };
	// limit intersection
	//ASSERT_TRUE(ray.IntersectSphere(sphere));

	origin = Vector3f{ -1.0f,-3.0f,-2.0f };
	ray = Ray3{ origin,direction };
	// no intersection
	//ASSERT_FALSE(ray.IntersectSphere(sphere));
}

} // namespace maths

