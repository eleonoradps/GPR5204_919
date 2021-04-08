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

#include <math.h>
#include "maths/vector3.h"
#include "aabb3.h"
#include "raytracing/material.h"

namespace maths {
class Sphere {
public:
    Sphere() = default;

    Sphere(float radius, maths::Vector3f center)
        : radius_(radius),
          center_(center) {
    };
    ~Sphere() = default;

    float area() const {
        const double pi = 3.14159265358979323846;
        return 4 * pi * (radius_ * radius_);
    }

    float volume() const {
        const double pi = 3.14159265358979323846;
        return 4 / 3 * pi * (radius_ * radius_ * radius_);
    }

    void set_radius(float radius) { radius_ = radius; }
    Vector3f center() const { return center_; }
    float radius() const { return radius_; }
    Material material() const { return material_; }
    void set_material(Material material) { material_ = material; }

private:
    Vector3f center_ = {};
    float radius_ = {};
    Material material_ = {};
};

bool OverlapSphere(const Sphere& a, const Sphere& b);
bool ContainSphere(const Sphere& a, const Sphere& b);
bool AABBOverlapSphere(const AABB3& a, const Sphere& b);
bool SphereContainAABB(const Sphere& sphere, const AABB3& aabb);
bool AABBContainSphere(const Sphere& sphere, const AABB3& aabb);

}  // namespace maths
