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

#include "maths/aabb3.h"
#include "maths/sphere.h"
#include "maths/vector3.h"
#include <algorithm>

namespace maths {

bool Overlap(const AABB3& a, const AABB3& b) {
	const Vector3f v1 = b.bottom_left() - a.top_right();
	const Vector3f v2 = a.bottom_left() - b.top_right();

	if (Contain(a, b) || Contain(b, a)) return false;
	return !(v1.x > 0 || v2.x > 0 || v1.y > 0 || v2.y > 0 ||
		v1.z > 0 || v2.z > 0);
}

bool Contain(const AABB3& a, const AABB3& b) {
	const Vector3f v1 = b.bottom_left() - a.bottom_left();
	const Vector3f v2 = a.top_right() - b.top_right();

	return (v1.x > 0 && v1.y > 0 && v1.z > 0 && v2.x > 0 
		&& v2.y > 0 && v2.z > 0);
}

bool OverlapSphere(const Sphere& a, const Sphere& b) {
	const Vector3f d = b.center() - a.center();
	const float v1 = b.radius() + a.radius();
	const float v2 = d.Magnitude();

	if (ContainSphere(a, b) || ContainSphere(b, a)) return false;
	return (v1 > v2);
}

bool ContainSphere(const Sphere& a, const Sphere& b) {
	const Vector3f d = b.center() - a.center();
	const float v1 = d.Magnitude() + b.radius();
	const float v2 = a.radius();

	return (v1 < v2);
}

bool AABBOverlapSphere(const AABB3& a, const Sphere& b) {
    // Vector from a to b absolute
    Vector3f n = b.center() - a.center();

    // Closest point on a to center b
    Vector3f closest = n;

    // Calculate half extents along each axis
    float x_extent = (a.top_right().x - a.bottom_left().x) / 2;
    float y_extent = (a.top_right().y - a.bottom_left().y) / 2;
    float z_extent = (a.top_right().z - a.bottom_left().z) / 2;
    Vector3f aabb_center = a.center();

    // Clamp point to edges of AABB
    closest.x = std::clamp(closest.x, - x_extent, + x_extent);
    closest.y = std::clamp(closest.y, - y_extent, + y_extent);
    closest.z = std::clamp(closest.z, - z_extent, + z_extent);
 
    float radius = b.radius();
    Vector3f difference = a.center() + closest;
    closest = difference - b.center();

    return closest.SqrMagnitude() <= radius *radius ;
}

bool SphereContainAABB(const Sphere& sphere, const AABB3& aabb) {
    float distance = 0;

    float x_max = std::abs(sphere.center().x - aabb.top_right().x);
    float x_min = std::abs(sphere.center().x - aabb.bottom_left().x);
    distance += std::powf(std::max(x_max, x_min), 2);

    float y_max = std::abs(sphere.center().y - aabb.top_right().y);
    float y_min = std::abs(sphere.center().y - aabb.bottom_left().y);
    distance += std::powf(std::max(y_max, y_min), 2);

    float z_max = std::abs(sphere.center().z - aabb.top_right().z);
    float z_min = std::abs(sphere.center().z - aabb.bottom_left().z);
    distance += std::powf(std::max(z_max, z_min), 2);

    if (distance < sphere.radius() * sphere.radius()) {
        return true;
    } else {
        return false;
    }
}

bool AABBContainSphere(const Sphere& sphere, const AABB3& aabb) {
    const AABB3 sphereAABB =
        AABB3(sphere.center() + Vector3f(sphere.radius(), sphere.radius(), sphere.radius()),
              sphere.center() - Vector3f(sphere.radius(), sphere.radius(), sphere.radius()));

    return Contain(aabb, sphereAABB);
}

}  // namespace maths