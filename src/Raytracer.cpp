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

#include "Raytracer.h"
#include <omp.h>
#include <cassert>

bool Raytracer::ObjectIntersect(maths::Ray3& ray,
	std::vector<maths::Sphere>& spheres, maths::Plane plane, Material& material, hitInfos& hitInfo)
{
	maths::Vector3f tmpHitPosition;
	for (int i = 0; i < spheres.size(); ++i)
	{
		if (ray.IntersectSphere(spheres[i],hitInfo.hitPosition))
		{
			hitInfo.normal= maths::Vector3f(hitInfo.hitPosition - spheres[i].center()).Normalized();
			//normal = maths::Vector3f(spheres[i].center() - ray.hit_position() / spheres[i].radius()).Normalized;
			material = spheres[i].material();
			return true;
		}
	}
	/*if(ray.IntersectPlane(plane,tmpHitPosition))
	{
		material = plane.material();
		return true;
	}*/
	return false;
}

maths::Vector3f Raytracer::RayCast(maths::Vector3f cameraOrigin,
	maths::Vector3f rayDirection, std::vector<maths::Sphere>& spheres, maths::Plane scenePlane, Light light)
{
	maths::Ray3 ray{ cameraOrigin, rayDirection };
	Material hitObject_material;
	hitInfos hitInfo;
	if (!ObjectIntersect(ray, spheres, scenePlane, hitObject_material, hitInfo))
	{
		return backgroundColor_;
	}
	else
	{
		maths::Vector3f light_direction{ light.position - hitInfo.hitPosition };
		//maths::Vector3f light_direction = light.direction;
		light_direction.Normalize();
		maths::Vector3f inverseLightDirection = maths::Vector3f(
			(-1) * light_direction.x, (-1) * light_direction.y, (-1) * light_direction.z);

		//Compute shadow ray to check if point is in shadow
		maths::Vector3f shadowRayOrigin(hitInfo.hitPosition + hitInfo.normal*1e-4);
		maths::Ray3 shadowRay(shadowRayOrigin, inverseLightDirection);
		Material tmpMaterial;
		hitInfos shadowHitInfo;
		if(ObjectIntersect(shadowRay,spheres,scenePlane,tmpMaterial,shadowHitInfo))
		{
			return hitObject_material.color() * 0.0f;
		}

		//Point is not in the shadow
		//
		//float dt = (light.position - ray.hit_position()).Magnitude();
		maths::Vector3f inverseDirection = maths::Vector3f((-1) * rayDirection.x, (-1)
			* rayDirection.y, (-1) + rayDirection.z);

		float facingRatio = std::max(0.0f, hitInfo.normal.Dot(inverseLightDirection));
		//float facingRatio = std::max(0.0f, hitInfo.normal.Dot(inverseDirection));
		
		//return (hitObject_material.color() * dt) * 0.5f;
		
		//return (hitObject_material.color() * facingRatio);
		return hitObject_material.color();
	}
}

void Raytracer::Render(float width, float height, float fov, std::vector<maths::Sphere>& spheres,maths::Plane scenePlane, Light light)
{
	int total = width * height;
	std::vector<maths::Vector3f> frameBuffer(total); // will hold the color for each pixel;
	
	#pragma omp parallel for
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			float dir_x = (j + 0.5f) - width / 2.;
			float dir_y = -(i + 0.5f) + height / 2.;
			float dir_z = -height / (2.0 * tan(fov / 2.0));

			maths::Vector3f rayDirection = maths::Vector3f(dir_x, dir_y, dir_z).Normalized();

			frameBuffer[ j + i *width] = RayCast(maths::Vector3f(0.0f, 0.0f, 0.0f),
				rayDirection, spheres,scenePlane, light);
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
