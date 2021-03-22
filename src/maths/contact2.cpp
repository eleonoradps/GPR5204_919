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

#include "maths/aabb2.h"
#include "maths/circle.h"
#include <algorithm>

namespace maths {

bool Overlap(const AABB2& a, const AABB2& b) {
    const Vector2f v1 = b.bottom_left() - a.top_right();
    const Vector2f v2 = a.bottom_left() - b.top_right();

    if (Contain(a, b) || Contain(b, a)) return false;
    return !(v1.x > 0 || v2.x > 0 || v1.y > 0 || v2.y > 0);
}

bool Contain(const AABB2& a, const AABB2& b) {
    const Vector2f v1 = b.bottom_left() - a.bottom_left();
    const Vector2f v2 = a.top_right() - b.top_right();

    return (v1.x > 0 && v1.y > 0 && v2.x > 0 && v2.y > 0);
}

bool OverlapCircle(const Circle& a, const Circle& b) {
    const Vector2f d = b.center() - a.center();
    const float r1 = b.radius() + a.radius();
    const float r2 = d.Magnitude();

    if (ContainCircle(a, b) || ContainCircle(b, a)) return false;
    return (r1 > r2);
}

bool ContainCircle(const Circle& a, const Circle& b) {
    const Vector2f d = b.center() - a.center();
    const float r1 = d.Magnitude() + b.radius();
    const float r2 = a.radius();

    return (r1 < r2);
}

bool AABBOverlapCircle(const AABB2& a, const Circle& b) {
    // Vector from a to b absolute
    Vector2f n = b.center() - a.center();

    // Closest point on a to center b
    Vector2f closest = n;

    // Calculate half extents along each axis
    float x_extent = (a.top_right().x - a.bottom_left().x) / 2;
    float y_extent = (a.top_right().y - a.bottom_left().y) / 2;
    Vector2f aabb_center = a.center();

    // Clamp point to edges of AABB
    closest.x =
        std::clamp(closest.x, - x_extent, + x_extent);
    closest.y =
        std::clamp(closest.y, - y_extent, + y_extent);

    float radius = b.radius();
    Vector2f difference = a.center() + closest;
    closest = difference - b.center();
    return closest.SqrMagnitude() <= (radius * radius);
}

bool CircleContainAABB(const Circle& circle, const AABB2& aabb) {
    float distance = 0;

    float x_max = std::abs(circle.center().x - aabb.top_right().x);
    float x_min = std::abs(circle.center().x - aabb.bottom_left().x);
    distance += std::powf(std::max(x_max, x_min), 2);

    float y_max = std::abs(circle.center().y - aabb.top_right().y);
    float y_min = std::abs(circle.center().y - aabb.bottom_left().y);

    distance += std::powf(std::max(y_max, y_min), 2);

    if (distance < circle.radius() * circle.radius()) {
        return true;
    } else {
        return false;
    }
}

bool AABBContainCircle(const Circle& circle, const AABB2& aabb) {
    const AABB2 circleAABB = AABB2(
        circle.center() + Vector2f(circle.radius(), circle.radius()),
        circle.center() - Vector2f(circle.radius(), circle.radius()));

    return Contain(aabb, circleAABB);
}

}  // namespace maths
