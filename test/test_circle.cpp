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

#include "maths/circle.h"
#include "maths/contact2.h"
#include <gtest/gtest.h>

namespace maths {

TEST(Maths, Circle_Overlap) {
    // Test of 2 overlapping circles
    Circle circle1(1.0f, Vector2f{0.0f, 0.0f});
    Circle circle2(1.0f, Vector2f{1.0f, 0.0f});
    EXPECT_TRUE(OverlapCircle(circle1, circle2));

    // Test of 2 circles that do not touch each other
    circle2 = Circle(1.0f, Vector2f{3.0f, 0.0f});
    EXPECT_FALSE(OverlapCircle(circle1, circle2));

    // Test of 2 same circles
    circle2 = Circle(1.0f, Vector2f{0.0f, 0.0f});
    EXPECT_TRUE(OverlapCircle(circle1, circle2));

    // Test of one circle contained in the other
    circle2 = Circle(0.4f, Vector2f{0.5f, 0.0f});
    EXPECT_FALSE(OverlapCircle(circle1, circle2));

    // Test 2 circles touching each other
    circle2 = Circle(1.0f, Vector2f{1.0f, 0.0f});
    EXPECT_TRUE(OverlapCircle(circle1, circle2));
}

TEST(Maths, Circle_Contain) {
    // Test of 2 overlapping circles
    Circle circle1(1.0f, Vector2f{0.0f, 0.0f});
    Circle circle2(1.0f, Vector2f{1.0f, 0.0f});
    EXPECT_FALSE(ContainCircle(circle1, circle2));

    // Test of 2 circles that do not touch each other
    circle2 = Circle(1.0f, Vector2f{3.0f, 0.0f});
    EXPECT_FALSE(ContainCircle(circle1, circle2));

    // Test of 2 same circles
    circle2 = Circle(1.0f, Vector2f{0.0f, 0.0f});
    EXPECT_FALSE(ContainCircle(circle1, circle2));

    // Test of one circle contained in the other
    circle2 = Circle(0.4f, Vector2f{0.5f, 0.0f});
    EXPECT_TRUE(ContainCircle(circle1, circle2));

    // Test 2 circles touching each other
    circle2 = Circle(1.0f, Vector2f{1.0f, 0.0f});
    EXPECT_FALSE(ContainCircle(circle1, circle2));
}

TEST(Maths, Circle_Contain_AABB2) {
    // Test AABB is inside circle
    Circle circle(0.5f, Vector2f{0.5f, 0.5f});
    AABB2 aabb(Vector2f{0.2f, 0.2f}, Vector2f{0.8f, 0.8f});
    EXPECT_TRUE(CircleContainAABB(circle, aabb));

    // Test of AABB corners outside the circle
    aabb = AABB2(Vector2f{0.0f, 0.0f}, Vector2f{1.0f, 1.0f});
    EXPECT_FALSE(CircleContainAABB(circle, aabb));
}

TEST(Maths, Circle_Overlap_AABB2) {
    // Test of a corner of the aabb outside the circle
    Circle circle(1.0f, Vector2f{0.0f, 0.0f});
    AABB2 aabb(Vector2f{0.0f, 0.0f}, Vector2f{1.0f, 1.0f});
    EXPECT_TRUE(AABBOverlapCircle(aabb, circle));

    // Test the circle is outside the AABB
    circle = Circle(0.25f, Vector2f{0.7f, 0.3f});
    aabb = AABB2(Vector2f{-0.6f, 0.6f}, Vector2f{0.4f, 1.6f});
    EXPECT_FALSE(AABBOverlapCircle(aabb, circle));

    // Test the circle overlaps the aabb but the corners of the aabb are outside the circle
    circle = Circle(0.5f, Vector2f{0.5f, 0.5f});
    aabb = AABB2(Vector2f{0.8f, 0.0f}, Vector2f{1.8f, 1.0f});
    EXPECT_TRUE(AABBOverlapCircle(aabb, circle));

    // Test the AABB is inside the circle
    aabb = AABB2(Vector2f{0.2f, 0.2f}, Vector2f{0.8f, 0.8f});
    EXPECT_TRUE(AABBOverlapCircle(aabb, circle));

    // Test the circle is inside the AABB
    circle = Circle(0.4f, Vector2f{0.5f, 0.5f});
    aabb = AABB2(Vector2f{0.0f, 0.0f}, Vector2f{1.0f, 1.0f});
    EXPECT_TRUE(AABBOverlapCircle(aabb, circle));

    // Test that the edge of the AABB touches the edge of the circle
    circle = Circle(0.5f, Vector2f{0.5f, 0.5f});
    aabb = AABB2(Vector2f{1.0f, 0.0f}, Vector2f{2.0f, 1.0f});
    EXPECT_TRUE(AABBOverlapCircle(aabb, circle));
}

TEST(Maths, AABB2_Contain_Circle) {
    // Test that the circle overlaps the AABB without touching the corners
    Circle circle = Circle(0.4f, Vector2f{0.5f, 0.5f});
    AABB2 aabb = AABB2(Vector2f{0.0f, 0.0f}, Vector2f{1.0f, 1.0f});
    EXPECT_FALSE(CircleContainAABB(circle, aabb));
}

}  // namespace
