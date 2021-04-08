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

#include "maths/ray3.h"

namespace maths {

bool Ray3::IntersectSphere(const Sphere& sphere, Vector3f& hitPosition, float& distance) {

    const Vector3f v = sphere.center() - origin_;
    const float d = v.Dot(direction_); // Distance to closest point to sphere center
    //float distance;
    if (d < 0) {
        return false;
    }

    const float squaredDistance = v.Dot(v) - (d * d); // squared Distance between closest point to sphere center
    const float radius2 = sphere.radius() * sphere.radius();
    if (squaredDistance > radius2) {
        return false;
    }

    const auto q = std::sqrt(radius2 - squaredDistance);

    const auto t0 = d + q;
    const auto t1 = d - q;

    bool hasHit = false;
    if (t0 >= 0) {
        distance = t0;
        hasHit = true;
    }

    if (t1 >= 0) {
        if (!hasHit || t1 < distance) {
            distance = t1;
            hasHit = true;
        }
    }

    if (!hasHit) {
        return false;
    }

    // calculate the position where the ray hit
    hitPosition = origin_ + direction_ * distance;
    
    
    //const Vector3f origin = origin_;
    //const Vector3f direction = direction_;
    //const Vector3f oc = origin - sphere.center();
    //const double b = 2 * Vector3f::Dot(oc, direction);
    //const double c = Vector3f::Dot(oc, oc) - sphere.radius() * sphere.radius();
    //double disc = b * b - 4 * c;
    //if (disc < 1e-4) return false;
    //disc = sqrt(disc);
    //double t0 = -b - disc;
    //double t1 = -b + disc;
    //if (t0 < 0) t0 = t1;
    //if (t0 < 0) return false;
    //distance = t0;
    ////distance = (t0 < t1) ? t0 : t1;
    //hit_position = origin_ + direction_ * distance;
    //
    return true;

}

bool Ray3::IntersectAABB3(const AABB3& aabb) {

    const Vector3f lb = aabb.bottom_left();
    const Vector3f rt = aabb.top_right();
    Vector3f dirfrac;
    float distance;

    dirfrac.x = 1.0f / direction().x;
    dirfrac.y = 1.0f / direction().y;
    dirfrac.z = 1.0f / direction().z;
    // lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
    const float t1 = (lb.x - origin_.x) * dirfrac.x;
    const float t2 = (rt.x - origin_.x) * dirfrac.x;
    const float t3 = (lb.y - origin_.y) * dirfrac.y;
    const float t4 = (rt.y - origin_.y) * dirfrac.y;
    const float t5 = (lb.z - origin_.z) * dirfrac.z;
    const float t6 = (rt.z - origin_.z) * dirfrac.z;

    const float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)),
                          std::min(t5, t6));
    const float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)),
                          std::max(t5, t6));

    // if tmax < 0, ray is intersecting AABB, but the whole AABB is behind
    if (tmax < 0) {
        return false;
    }

    // if tmin > tmax, ray doesn't intersect AABB
    if (tmin > tmax) {
        return false;
    }

    distance = tmin;

    // calculate the position where the ray hit
    hit_position_ = origin_ + direction_ * distance;
    return true;
}

bool Ray3::IntersectPlane(const Plane& plane, Vector3f& hitPosition) {
    const float s = direction_.Dot(plane.normal());
    if (s > 0) {
        return false;
    }
    const float distance = (plane.Distance(plane.point()) - origin_.Dot(
                          plane.normal())) / s;
    if (distance < 0) {
        return false;
    }

    // calculate the position where the ray hit
    hitPosition = origin_ + (direction_ * distance);
    return true;
}

} // namespace maths
