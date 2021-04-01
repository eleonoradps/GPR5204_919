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

#include <vector>
#include <fstream>
#include <algorithm>

#include "maths/vector3.h"
#include "maths/sphere.h"
#include "maths/ray3.h"
#include "maths/plane.h"


struct Light {
	Light() = default;
	//maths::Vector3f position{ 20.0f,20.0f,20.0f };
	maths::Vector3f position{ 35.0f,0.0f,-16.0f };
	maths::Vector3f direction{ 1.0f,0.0f,0.0f };
};

struct hitInfos
{
	maths::Vector3f normal;
	maths::Vector3f hitPosition;
	float distance;
};

class Raytracer {
public:

	/*maths::Vector3f Reflect();*/

	bool ObjectIntersect(maths::Ray3& ray, std::vector<maths::Sphere>& spheres, maths::Plane plane, Material& material, hitInfos& hitInfos, float& distance);
	
	maths::Vector3f RayCast(maths::Vector3f cameraOrigin, maths::Vector3f rayDirection, std::vector<maths::Sphere> &spheres, maths::Plane scenePlane, Light light);

	void Render(int width, int height, float fov, std::vector<maths::Sphere>& spheres, maths::Plane scenePlane,Light light);

	void SceneGeneration(const int sphereNumber, const float sphereMaxSize, const float sphereMinSize);
	
private:
	maths::Vector3f backgroundColor_{ 150.0f,200.0f,255.0f };
};