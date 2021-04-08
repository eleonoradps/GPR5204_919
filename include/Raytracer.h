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


struct PointLight {
	maths::Vector3f position{ 10.0f,10.0f,0.0f };
};

struct hitInfos
{
	maths::Vector3f normal;
	maths::Vector3f hitPosition;
	float distance;
};

class Raytracer {
public:

	//Set bases value and variable for raytracer rendering
	void SetScene(std::vector<maths::Sphere>& spheres, std::vector<maths::Plane>& planes, PointLight light, int heigth, int width, float fov)
	{
		spheres_ = spheres;
		planes_ = planes;
		light_ = light;
		heigth_ = heigth;
		width_ = width;
		fov_ = fov;
		int total = width_ * heigth_;
		frameBuffer_ = std::vector<maths::Vector3f>(total);
	}

	//Cast ray for each pixel to check collision and render objects
	maths::Vector3f RayCast(maths::Vector3f cameraOrigin, maths::Vector3f rayDirection,  const int& depth = 0);

	//Check intersection between the ray and each object in the scene
	bool ObjectIntersect(maths::Ray3& ray, Material& hitMaterial, hitInfos& hitInfos, float& distance);

	//Cast a shadow ray to check intersection with objects and render shadows
	bool ShadowRay(maths::Vector3f hitPosition, maths::Vector3f hitNormal, maths::Vector3f lightNormal);

	maths::Vector3f Reflect(maths::Vector3f& rayDirection, maths::Vector3f& hitNormal);
	
	//Base raytracing function that will start raytracing rendering
	void Render();

	//Write scene result into a .ppm image
	void WriteImage();

	std::vector<maths::Vector3f> frameBuffer() const { return frameBuffer_; }
	
private:
	maths::Vector3f backgroundColor_{ 150.0f,200.0f,255.0f };
	std::vector<maths::Sphere> spheres_;
	std::vector<maths::Plane> planes_;
	PointLight light_;
	int heigth_;
	int width_;
	float fov_;
	std::vector<maths::Vector3f> frameBuffer_;
};