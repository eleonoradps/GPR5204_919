#pragma once

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

#include "maths/sphere.h"
#include "maths/contact3.h"
#include <gtest/gtest.h>

namespace maths {

TEST(Maths, Sphere_Overlap) {
    // Test of 2 overlaped spheres
    Sphere sphere1(1.0f, Vector3f{0.0f, 0.0f, 0.0f});
    Sphere sphere2(1.0f, Vector3f{1.0f, 0.0f, 0.0f});
    EXPECT_TRUE(OverlapSphere(sphere1, sphere2));

    // Test of 2 spheres that do not touch each other
    sphere2 = Sphere(1.0f, Vector3f{3.0f, 0.0f, 0.0f});
    EXPECT_FALSE(OverlapSphere(sphere1, sphere2));

    // Test of 2 same spheres
    sphere2 = Sphere(1.0f, Vector3f{0.0f, 0.0f, 0.0f});
    EXPECT_TRUE(OverlapSphere(sphere1, sphere2));

    // Test of one sphere contained in the other
    sphere2 = Sphere(0.4f, Vector3f{0.5f, 0.0f, 0.0f});
    EXPECT_FALSE(OverlapSphere(sphere1, sphere2));

    // Test of 2 spheres touching each other
    sphere2 = Sphere(1.0f, Vector3f{1.0f, 0.0f, 0.0f});
    EXPECT_TRUE(OverlapSphere(sphere1, sphere2));
}

TEST(Maths, Sphere_Contain) {
    // Test of 2 overlaped spheres
    Sphere sphere1(1.0f, Vector3f{0.0f, 0.0f, 0.0f});
    Sphere sphere2(1.0f, Vector3f{1.0f, 0.0f, 0.0f});
    EXPECT_FALSE(ContainSphere(sphere1, sphere2));

    // Test of 2 spheres that do not touch each other
    sphere2 = Sphere(1.0f, Vector3f{3.0f, 0.0f, 0.0f});
    EXPECT_FALSE(ContainSphere(sphere1, sphere2));

    // Test of 2 same spheres
    sphere2 = Sphere(1.0f, Vector3f{0.0f, 0.0f, 0.0f});
    EXPECT_FALSE(ContainSphere(sphere1, sphere2));

    // Test of one sphere contained in the other
    sphere2 = Sphere(0.4f, Vector3f{0.5f, 0.0f, 0.0f});
    EXPECT_TRUE(ContainSphere(sphere1, sphere2));

    // Test of 2 spheres touching each other
    sphere2 = Sphere(1.0f, Vector3f{1.0f, 0.0f, 0.0f});
    EXPECT_FALSE(ContainSphere(sphere1, sphere2));
}

TEST(Maths, Sphere_Contain_AABB3) {
    // Test AABB is inside sphere
    Sphere sphere(0.5f, Vector3f{0.5f, 0.5f, 1.0f});
    AABB3 aabb(Vector3f{0.2f, 0.2f, 1.0f}, Vector3f{0.8f, 0.8f, 1.0f});
    EXPECT_TRUE(SphereContainAABB(sphere, aabb));

    // Test of AABB corners outside the sphere
    aabb = AABB3(Vector3f{0.0f, 0.0f, 1.0f}, Vector3f{1.0f, 1.0f, 1.0f});
    EXPECT_FALSE(SphereContainAABB(sphere, aabb));
}

TEST(Maths, Sphere_Overlap_AABB3) {
    // Test of a corner of the aabb outside the sphere
    Sphere sphere(1.0f, Vector3f{0.0f, 0.0f, 1.0f});
    AABB3 aabb(Vector3f{0.0f, 0.0f, 1.0f}, Vector3f{1.0f, 1.0f, 2.0f});
    EXPECT_TRUE(AABBOverlapSphere(aabb, sphere));

    // Test the sphere is outside the AABB
    sphere = Sphere(0.25f, Vector3f{0.7f, 0.3f, 1.0f});
    aabb = AABB3(Vector3f{-0.6f, 0.6f, 1.0f}, Vector3f{0.4f, 1.6f, 1.0f});
    EXPECT_FALSE(AABBOverlapSphere(aabb, sphere));

    // Test the sphere overlaps the aabb but the corners of the aabb are outside
    // the sphere
    sphere = Sphere(0.5f, Vector3f{0.5f, 0.5f, 1.0f});
    aabb = AABB3(Vector3f{0.8f, 0.0f, 1.0f}, Vector3f{1.8f, 1.0f, 1.0f});
    EXPECT_TRUE(AABBOverlapSphere(aabb, sphere));

    // Test the AABB is inside the sphere
    aabb = AABB3(Vector3f{0.2f, 0.2f, 1.0f}, Vector3f{0.8f, 0.8f, 1.0f});
    EXPECT_TRUE(AABBOverlapSphere(aabb, sphere));

    // Test the sphere is inside the AABB
    sphere = Sphere(0.4f, Vector3f{0.5f, 0.5f, 1.0f});
    aabb = AABB3(Vector3f{0.0f, 0.0f, 1.0f}, Vector3f{1.0f, 1.0f, 1.0f});
    EXPECT_TRUE(AABBOverlapSphere(aabb, sphere));

    // Test that the edge of the AABB touches the edge of the sphere
    sphere = Sphere(0.5f, Vector3f{0.5f, 0.5f, 1.0f});
    aabb = AABB3(Vector3f{1.0f, 0.0f, 1.0f}, Vector3f{2.0f, 1.0f, 1.0f});
    EXPECT_TRUE(AABBOverlapSphere(aabb, sphere));
}

TEST(Maths, AABB3_Contain_Sphere) {
    // Test that the sphere overlaps the AABB without touching the corners
    Sphere sphere = Sphere(0.4f, Vector3f{0.5f, 0.5f, 1.0f});
    AABB3 aabb = AABB3(Vector3f{0.0f, 0.0f, 1.0f}, Vector3f{1.0f, 1.0f, 1.0f});
    EXPECT_FALSE(SphereContainAABB(sphere, aabb));
}

}  // namespace maths
