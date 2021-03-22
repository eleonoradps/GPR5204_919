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
#include "maths/vector2.h"

namespace maths {
TEST(Maths, Vector2f_Addition) {
    const Vector2f a{2.0f, 3.0f};
    const Vector2f b{1.0f, 4.0f};

    //Test operator +.
    Vector2f c = a + b;
    EXPECT_EQ(c.x, a.x + b.x);
    EXPECT_EQ(c.y, a.y + b.y);

    //Test operator +=.
    const Vector2f d{3.0f, 4.0f};
    Vector2f e = d;
    e += a;
    EXPECT_EQ(e.x, d.x + a.x);
    EXPECT_EQ(e.y, d.y + a.y);
}

TEST(Maths, Vector2f_Substraction) {
    const Vector2f a{2.0f, 3.0f};
    const Vector2f b{1.0f, 4.0f};

    //Test operator -.
    Vector2f c = a - b;
    EXPECT_EQ(c.x, a.x - b.x);
    EXPECT_EQ(c.y, a.y - b.y);

    //Test operator -=.
    const Vector2f d{3.0f, 4.0f};
    Vector2f e = d;
    e -= a;
    EXPECT_EQ(e.x, d.x - a.x);
    EXPECT_EQ(e.y, d.y - a.y);
}

TEST(Maths, Vector2f_MultiplicationByScalar) {
    const Vector2f a{2.0f, 3.0f};
    const float b = 4.0f;

    //Test operator +.
    Vector2f c = a * b;
    EXPECT_EQ(c.x, a.x * b);
    EXPECT_EQ(c.y, a.y * b);

    //Test operator +=.
    const Vector2f d{3.0f, 4.0f};
    Vector2f e = d;
    e *= b;
    EXPECT_EQ(e.x, d.x * b);
    EXPECT_EQ(e.y, d.y * b);
}

TEST(Maths, Vector2f_DivisionByScalar) {
    const Vector2f a{2.0f, 3.0f};
    const float b = 4.0f;

    //Test operator /.
    Vector2f c = a / b;
    EXPECT_EQ(c.x, a.x / b);
    EXPECT_EQ(c.y, a.y / b);

    //Test operator /=.
    const Vector2f d{3.0f, 4.0f};
    Vector2f e = d;
    e /= b;
    EXPECT_EQ(e.x, d.x / b);
    EXPECT_EQ(e.y, d.y / b);
}

TEST(Maths, Vector2f_Equal) {
    const Vector2f a{ 2.0f, 3.0f };
    const Vector2f b = a;

    //Test operator ==.
    EXPECT_TRUE(a.x == b.x);
    EXPECT_TRUE(a.y == b.y);
}

TEST(Maths, Vector2f_NotEqual) {
    const Vector2f a{ 2.0f, 3.0f };
    const Vector2f b{ 1.0f, 4.0f };

    //Test operator !=.
    EXPECT_TRUE(a.x != b.x);
    EXPECT_TRUE(a.y != b.y);
}

TEST(Maths, Vector2f_SubscriptOperator) {
    const Vector2f a{ 2.0f, 3.0f };

    //Test operator [].
    EXPECT_EQ(a[0], 2.0f);
    EXPECT_EQ(a[1], 3.0f);
}

TEST(Maths, Vector2f_Magnitude) {
    const Vector2f a{2.0f, 3.0f};

    //Test .Magnitude().
    EXPECT_EQ(a.Magnitude(), std::sqrt((a.x * a.x) + (a.y * a.y)));
}

TEST(Maths, Vector2f_SqrMagnitude) {
    const Vector2f a{2.0f, 3.0f};

    //Test .SqrMagnitude().
    EXPECT_EQ(a.SqrMagnitude(), (a.x * a.x) + (a.y * a.y));
}

TEST(Maths, Vector2f_DotProduct) {
    const Vector2f a{0.0f, 3.0f};
    const Vector2f b{1.0f, 0.0f};

    //Test .Dot() .
    EXPECT_EQ(a.Dot(b), 0.0f);

    //Test static Dot().
    EXPECT_EQ(maths::Vector2f::Dot(a, b), 0.0f);

    const Vector2f c{2.0f, 3.0f};
    const Vector2f d{1.0f, 1.0f};

    //Test .Dot().
    EXPECT_EQ(c.Dot(d), c.x * d.x + c.y * d.y);

    //Test static Dot().
    EXPECT_EQ(maths::Vector2f::Dot(c, d), c.x * d.x + c.y * d.y);
}

TEST(Maths, Vector2f_CrossProduct) {
    const Vector2f a{2.0f, 3.0f};
    const Vector2f b{1.0f, -3.0f};

    //Test .Cross().
    EXPECT_EQ(a.Cross(b).x, 0);
    EXPECT_EQ(a.Cross(b).y, 0);
    EXPECT_EQ(a.Cross(b).z, a.x * b.y - a.y * b.x);

    //Test static Cross().
    EXPECT_EQ(maths::Vector2f::Cross(a, b).x, 0);
    EXPECT_EQ(maths::Vector2f::Cross(a, b).y, 0);
    EXPECT_EQ(maths::Vector2f::Cross(a, b).z, a.x * b.y - a.y * b.x);
}

TEST(Maths, Vector2f_AngleBetween) {
    const Vector2f a{0.0f, 3.0f};
    const Vector2f b{1.0f, 0.0f};

    //Test .AngleBetween().
    EXPECT_EQ(a.AngleBetween(b),
              maths::acos(a.Dot(b) / (a.Magnitude() * b.Magnitude())));

    //Test static AngleBetween().
    EXPECT_EQ(maths::Vector2f::AngleBetween(a, b),
              maths::acos(maths::Vector2f::Dot(a, b) / (a.Magnitude() *
                  b.Magnitude())));
}

TEST(Maths, Vector2f_Normalize) {
    const Vector2f a{0.0f, 3.0f};
    const Vector2f b = a.Normalized();

    //Test .Normalized().
    EXPECT_EQ(b.Magnitude(), 1.0f);

    //Test .Normalize().
    Vector2f c = a;
    c.Normalize();
    EXPECT_EQ(c.Magnitude(), 1.0f);
}

TEST(Maths, Vector2f_Lerp) {
    const Vector2f a{2.0f, 3.0f};
    const Vector2f b{1.0f, 4.0f};

    const float t0 = 0.0f;
    const float t1 = 1.0f;

    //Test static Vector2f Lerp t = 0.
    Vector2f c = Vector2f::Lerp(a, b, t0);
    EXPECT_EQ(c.x, a.x);
    EXPECT_EQ(c.y, a.y);

    //Test static Vector2f Lerp t = 1.
    Vector2f d = Vector2f::Lerp(a, b, t1);
    EXPECT_EQ(d.x, b.x);
    EXPECT_EQ(d.y, b.y);

    //Test Vector2f Lerp t = 0.
    Vector2f e = a.Lerp(b, t0);
    EXPECT_EQ(e.x, a.x);
    EXPECT_EQ(e.y, a.y);

    //Test Vector2f Lerp t = 1.
    Vector2f f = a.Lerp(b, t1);
    EXPECT_EQ(f.x, b.x);
    EXPECT_EQ(f.y, b.y);
}

TEST(Maths, Vector2f_Slerp) {
    float threshold = 0.0001f;

    const Vector2f a{2.0f, 3.0f};
    const Vector2f b{1.0f, 4.0f};

    const float t0 = 0.0f;
    const float t1 = 1.0f;

    //Test Vector2f Slerp t = 0.
    Vector2f c = a.Slerp(b, t0);
    //Check difference between result & expected value
    //Because slerp function lose too much precision with double.
    EXPECT_TRUE(std::abs(c.x - a.x) < threshold);
    EXPECT_TRUE(std::abs(c.y - a.y) < threshold);

    //Test Vector2f Slerp t = 1.
    Vector2f d = a.Slerp(b, t1);
    EXPECT_TRUE(std::abs(d.x - b.x) < threshold);
    EXPECT_TRUE(std::abs(d.y - b.y) < threshold);
}

TEST(Maths, Vector2f_Rotation) {
    const Vector2f a = Vector2f{1.0f, 3.0f};
    const degree_t b = degree_t(45.f);
    const radian_t c = b;

    //Test .Rotation().
    EXPECT_EQ(a.Rotation(b).x,
              (a.x * maths::cos(c)) + (a.y * -maths::sin(c)));
    EXPECT_EQ(a.Rotation(b).y,
              (a.x * maths::sin(c)) + (a.y * maths::cos(c)));

    //Test static Rotation() .
    EXPECT_EQ(maths::Vector2f::Rotation(a, b).x,
              (a.x * maths::cos(c)) + (a.y * -maths::sin(c)));
    EXPECT_EQ(maths::Vector2f::Rotation(a, b).y,
              (a.x * maths::sin(c)) + (a.y * maths::cos(c)));
}
} // namespace maths
