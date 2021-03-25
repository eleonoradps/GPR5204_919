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

# include "Raytracer.h"

bool Raytracer::ObjectIntersect(maths::Ray3& ray, 
	std::vector<maths::Sphere>& spheres, maths::Vector3f& normal, Material& material)
{
	for (int i = 0; i < spheres.size(); ++i)
	{
		if (ray.IntersectSphere(spheres[i]))
		{
			normal = maths::Vector3f(ray.hit_position() - spheres[i].center()).Normalized();
			//normal = maths::Vector3f(spheres[i].center() - ray.hit_position() / spheres[i].radius()).Normalized;
			material = spheres[i].material();
			return true;
		}
	}
	return false;
}

maths::Vector3f Raytracer::RayCast(maths::Vector3f cameraOrigin,
	maths::Vector3f rayDirection, std::vector<maths::Sphere> spheres, Light light)
{
	maths::Vector3f normal;
	maths::Ray3 ray{ cameraOrigin, rayDirection };
	Material hitObject_material;
	if (!ObjectIntersect(ray, spheres, normal, hitObject_material))
	{
		return backgroundColor_;
	}
	else
	{
		maths::Vector3f light_direction{ light.position - ray.hit_position() };
		float dt = (light.position - ray.hit_position()).Magnitude();
		maths::Vector3f inverseDirection = maths::Vector3f((-1) * rayDirection.x, (-1)
			* rayDirection.y, (-1) + rayDirection.z);
		float facingRatio = std::max(0.0f, normal.Dot(inverseDirection));
		//return (hitObject_material.color() * dt) * 0.5f;
		return (hitObject_material.color() * facingRatio) * hitObject_material.reflexionIndex();
	}
}

void Raytracer::Render(float width, float height, float fov, std::vector<maths::Sphere> spheres, Light light)
{
	std::vector<maths::Vector3f> frameBuffer(width * height); // will hold the color for each pixel;
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			float dir_x = (j + 0.5f) - width / 2.;
			float dir_y = -(i + 0.5f) + height / 2.;
			float dir_z = -height / (2.0 * tan(fov / 2.0));

			maths::Vector3f rayDirection = maths::Vector3f(dir_x, dir_y, dir_z).Normalized();

			frameBuffer[i + j * width] = RayCast(maths::Vector3f(0.0f, 0.0f, 0.0f),
				rayDirection, spheres, light);
		}
	}

	std::ofstream ofs("./image.ppm", std::ios::out | std::ios::binary);
	ofs << "P6\n" << width << " " << height << "\n255\n";
	for (uint32_t i = 0; i < height * width; ++i) {

		char r = (char)(frameBuffer[i].x);
		char g = (char)(frameBuffer[i].y);
		char b = (char)(frameBuffer[i].z);

		ofs << r << g << b;
	}

	ofs.close();
}
