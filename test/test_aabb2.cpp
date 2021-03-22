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
#include "maths/aabb2.h"

namespace maths {
TEST(Maths, Aabb2_Extent) {
    // Test extent of the aabb
    maths::AABB2 aabb_2d(Vector2f{0.0f, 1.0f}, Vector2f{1.0f, 2.0f});
    EXPECT_FLOAT_EQ(aabb_2d.extent().x, 0.5f);
    EXPECT_FLOAT_EQ(aabb_2d.extent().y, 0.5f);
}

TEST(Maths, Aabb2_Center) {
    // Test aabb center
    AABB2 aabb_2d(Vector2f{0.0f, 1.0f}, Vector2f{1.0f, 2.0f});
    EXPECT_FLOAT_EQ(aabb_2d.center().x, 0.5f);
    EXPECT_FLOAT_EQ(aabb_2d.center().y, 1.5f);
}

TEST(Maths, Aabb2_Overlap) {
    // Test if aabb2 is inside aabb1
    AABB2 aabb1(Vector2f{0.0f, 0.0f}, Vector2f{1.0f, 1.0f});
    AABB2 aabb2(Vector2f{0.1f, 0.1f}, Vector2f{0.9f, 0.9f});
    EXPECT_FALSE(Overlap(aabb1, aabb2));
    EXPECT_FALSE(Overlap(aabb2, aabb1));

    // Test if aabb2 is outside of aabb1 positive
    aabb2 = AABB2(Vector2f{1.1f, 1.1f}, Vector2f{1.5f, 1.5f});
    EXPECT_FALSE(Overlap(aabb1, aabb2));

    // Test if aabb2 is outside of aabb1 negative
    aabb2 = AABB2(Vector2f{-0.5f, -0.5f}, Vector2f{-0.1f, -0.1f});
    EXPECT_FALSE(Overlap(aabb1, aabb2));

    // Test if angle aabb2 is overlapped aabb1 in positive
    aabb2 = AABB2(Vector2f{1.0f, 1.0f}, Vector2f{1.5f, 1.5f});
    EXPECT_TRUE(Overlap(aabb1, aabb2));

    // Test if angle aabb2 is overlapped on aabb1 in negative
    aabb2 = AABB2(Vector2f{-0.5f, -0.5f}, Vector2f{-0.0f, -0.0f});
    EXPECT_TRUE(Overlap(aabb1, aabb2));

    // Test if bottom left corner of aabb2 overlaps aabb1
    aabb2 = AABB2(Vector2f{0.5f, 0.5f}, Vector2f{1.5f, 1.5f});
    EXPECT_TRUE(Overlap(aabb1, aabb2));

    // Test if top right angle of aabb2 overlaps aabb1
    aabb2 = AABB2(Vector2f{-0.5f, -0.5f},
                  Vector2f{0.5f, 0.5f});
    EXPECT_TRUE(Overlap(aabb1, aabb2));

    // Test if aabb2 crosses aabb1
    aabb2 = AABB2(Vector2f{-0.1f, 0.1f}, Vector2f{1.1f, 0.9f});
    EXPECT_TRUE(Overlap(aabb1, aabb2));

    // Test of the same aabb
    aabb2 = AABB2(Vector2f{0.0f, 0.0f}, Vector2f{1.0f, 1.0f});
    EXPECT_TRUE(Overlap(aabb1, aabb2));
}

TEST(Maths, Aabb2_Contain) {
    // Test if aabb2 is inside aabb1
    maths::AABB2 aabb1(Vector2f{0.0f, 0.0f}, Vector2f{1.0f, 1.0f});
    AABB2 aabb2(Vector2f{0.1f, 0.1f}, Vector2f{0.9f, 0.9f});
    EXPECT_TRUE(Contain(aabb1, aabb2));
    EXPECT_FALSE(Contain(aabb2, aabb1));

    // Test if aabb2 is outside of aabb1 positive
    aabb2 = AABB2(Vector2f{1.1f, 1.1f}, Vector2f{1.5f, 1.5f});
    EXPECT_FALSE(Contain(aabb1, aabb2));

    // Test if aabb2 is outside of aabb1 negative
    aabb2 = AABB2(Vector2f{-0.5f, -0.5f}, Vector2f{-0.1f, -0.1f});
    EXPECT_FALSE(Contain(aabb1, aabb2));

    // Test if angle aabb2 is overlapped aabb1 in positive
    aabb2 = AABB2(Vector2f{1.0f, 1.0f}, Vector2f{1.5f, 1.5f});
    EXPECT_FALSE(Contain(aabb1, aabb2));

    // Test if angle aabb2 is overlapped on aabb1 in negative
    aabb2 = AABB2(Vector2f{-0.5f, -0.5f}, Vector2f{-0.0f, -0.0f});
    EXPECT_FALSE(Contain(aabb1, aabb2));

    // Test if bottom left corner of aabb2 overlaps aabb1
    aabb2 = AABB2(Vector2f{0.5f, 0.5f}, Vector2f{1.5f, 1.5f});
    EXPECT_FALSE(Contain(aabb1, aabb2));

    // Test if top right angle of aabb2 overlaps aabb1
    aabb2 = AABB2(Vector2f{-0.5f, -0.5f}, Vector2f{0.5f, 0.5f});
    EXPECT_FALSE(Contain(aabb1, aabb2));

    // Test if aabb2 crosses aabb1
    aabb2 = AABB2(Vector2f{-0.1f, 0.1f}, Vector2f{1.1f, 0.9f});
    EXPECT_FALSE(Contain(aabb1, aabb2));

    // Test of the same aabb
    aabb2 = AABB2(Vector2f{0.0f, 0.0f}, Vector2f{1.0f, 1.0f});
    EXPECT_FALSE(Contain(aabb1, aabb2));
}

}  // namespace maths
