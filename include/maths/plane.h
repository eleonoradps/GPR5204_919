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

#include "maths/vector3.h"
#include "raytracing/material.h"

namespace maths
{
	
class Plane {

public:
	Plane() = default;
	Plane(const Vector3f& point, const Vector3f& normal)
	: point_(point), normal_(normal) {}
	Plane(Vector3f a, Vector3f b, Vector3f c)
	: point_(a), normal_(CalculateNormalFromPoints(a,b,c)) {}

	float Distance( const Vector3f& point) const {
		return {Vector3f::Dot(point - point_, normal_) };
	}

	// calculate the normal of the plane based on the three points
	Vector3f CalculateNormalFromPoints(Vector3f a, Vector3f b, Vector3f c) const {
		Vector3f v_a = a - b;
		Vector3f v_b = c - b;
		return { Vector3f::Cross(v_a,v_b).Normalized() };
	}
	
	Vector3f point() const { return { point_ }; }
	Vector3f normal() const { return { normal_ }; }
	void SetMaterial(Material material) { material_ = material; }
	Material material() const { return material_; }
	
private:
	Vector3f point_;
	Vector3f normal_;
	Material material_;
};

} // namespace maths