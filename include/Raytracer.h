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

#include "maths/vector3.h"
#include "maths/vector4.h"
#include "maths/sphere.h"
#include "maths/ray3.h"


class  Material {
public:
	Material() = default;
	Material(maths::Vector4f albedo, float reflexionIndex) : albedo_(albedo), reflexionIndex_(reflexionIndex) {}
	
private:
	maths::Vector3f color_;
	maths::Vector4f albedo_ = maths::Vector4f(1.0f, 0.0f, 0.0f, 0.0f);
	float reflexionIndex_;
};

struct Light
{
	
};

class Raytracer {
public:

	maths::Vector3f Reflect()
	{

		return;
	}

	bool ObjectIntersect(maths::Ray3 ray, std::vector<maths::Sphere> spheres, maths::Vector3f normal){}

	maths::Vector3f RayCast(maths::Vector3f cameraOrigin, maths::Vector3f rayDirection,std::vector<maths::Sphere>, Light light) {}


	void Render(float width, float height, float fov, std::vector<maths::Sphere> spheres, Light light )
	{
		std::vector<maths::Vector3f> frameBuffer(width * height); // will hold the color for each pixel;
		for(int i = 0; i<height; ++i)
		{
			for(int j = 0; j < width; ++j)
			{
				float dir_x = (i + 0.5) - width / 2.;
				float dir_y = -(j + 0.5) + height / 2.; 
				float dir_z = -height / (2. * tan(fov / 2.));
				maths::Vector3f rayDirection = maths::Vector3f(dir_x, dir_y, dir_z).Normalized();
				
				frameBuffer[i + j * width] = RayCast(maths::Vector3f(0.0f, 0.0f, 0.0f), rayDirection, spheres, light);
			}
		}

		// TO DO :
		// Use the frmabuffer data to draw the image;
	}

	
private:
	std::vector<maths::Vector3f> frameBuffer_;
};