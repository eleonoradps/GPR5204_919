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
#include <cmath>
#include "maths/vector2.h"
#include "aabb2.h"

namespace maths {

class Circle {
public:
    Circle() = default;

    Circle(float radius, Vector2f center)
        : radius_(radius),
          center_(center) {
    };
    ~Circle() = default;

    float area() const {
        const double pi = 3.14159265358979323846;
        return (pi * (radius_ * radius_));
    }

    void set_radius(const float radius) { radius_ = radius; }
    Vector2f center() const { return center_; }
    float radius() const { return radius_; }

private:
    Vector2f center_ = {};
    float radius_ = {};
};

bool OverlapCircle(const Circle& a, const Circle& b);
bool ContainCircle(const Circle& a, const Circle& b);
bool AABBOverlapCircle(const AABB2& a, const Circle& b);
bool CircleContainAABB(const Circle& circle, const AABB2& aabb);
bool AABBContainCircle(const Circle& circle, const AABB2& aabb);

}  // namespace maths
