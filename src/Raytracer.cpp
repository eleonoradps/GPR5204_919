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
	 Material& hitMaterial, hitInfos& hitInfo, float& distance)
{
	maths::Vector3f tmpHitPosition;
	distance = 1000000.0f;
	for (int i = 0; i < spheres_.size(); ++i)
	{
		hitInfo.distance = 10000000.0f;
		// Only render intersection for the nearest sphere
		if (ray.IntersectSphere(spheres_[i],hitInfo.hitPosition,hitInfo.distance) && hitInfo.distance< distance)
		{
			hitInfo.normal= maths::Vector3f(hitInfo.hitPosition - spheres_[i].center()).Normalized();
			hitMaterial = spheres_[i].material();
			distance = hitInfo.distance;
		}
	}
	if(distance!=1000000.0f)
	{
		return true;
	}	
	return false;
	
}

maths::Vector3f Raytracer::RayCast(maths::Vector3f cameraOrigin,
	maths::Vector3f rayDirection)
{
	maths::Ray3 ray{ cameraOrigin, rayDirection };
	Material hitObject_material;
	hitInfos hitInfo;
	float distance;
	bool inLight = true;
	
	if (!ObjectIntersect(ray,hitObject_material, hitInfo,distance))
	{
		return backgroundColor_;
	}
	else
	{
		maths::Vector3f light_direction{ light_.position - hitInfo.hitPosition };
		//light_direction.Normalize();

		maths::Vector3f inverseLightDirection = maths::Vector3f(
			(-1) * light_direction.x, (-1) * light_direction.y, (-1) * light_direction.z);
		inverseLightDirection.Normalize();
		
		maths::Vector3f light_normal(light_.position - hitInfo.hitPosition);
		light_normal.Normalize();
		
		//Compute shadow ray to check if point is in shadow
		inLight = ShadowRay(hitInfo.hitPosition, hitInfo.normal, light_normal);

		float ligth_value = maths::Vector3f::Dot(hitInfo.normal, light_normal);
		if(ligth_value< 0.0f)
		{
			ligth_value = 0.0f;
		}
		
		//Point is not in the shadow

		//float facingRatio = std::max(0.0f, hitInfo.normal.Dot(inverseLightDirection));
		hitObject_material.set_color(hitObject_material.color() * ligth_value * inLight);
		return (hitObject_material.color());
		
		
		//return hitObject_material.color();
	}
}

void Raytracer::Render()
{
	#pragma omp parallel for
	for (int i = 0; i < heigth_; ++i)
	{
		for (int j = 0; j < width_; ++j)
		{
			double dir_x = (j + 0.5f) - width_ / 2.0;
			double dir_y = -(i + 0.5f) + heigth_ / 2.0;
			double dir_z = -heigth_ / (2.0 * tan(fov_ / 2.0));

			maths::Vector3f rayDirection = maths::Vector3f(dir_x, dir_y, dir_z).Normalized();

			frameBuffer_[ j + i *width_] = RayCast(maths::Vector3f(0.0f, 0.0f, 0.0f),
				rayDirection);
		}
	}

	//WriteImage();
}

void Raytracer::WriteImage()
{
	std::ofstream ofs("./image.ppm", std::ios::out | std::ios::binary);
	ofs << "P6\n" << width_ << " " << heigth_ << "\n255\n";
	for (uint32_t i = 0; i < heigth_ * width_; ++i) {

		char r = (char)(frameBuffer_[i].x);
		char g = (char)(frameBuffer_[i].y);
		char b = (char)(frameBuffer_[i].z);

		ofs << r << g << b;
	}
	ofs.close();
}

bool Raytracer::ShadowRay(maths::Vector3f hitPosition, maths::Vector3f hitNormal, maths::Vector3f lightNormal)
{
	maths::Vector3f shadowRayOrigin(hitPosition + hitNormal * 1e-4);
	maths::Ray3 shadowRay(shadowRayOrigin, lightNormal);
	Material tmpMaterial;
	hitInfos shadowHitInfo;
	float tmpDistance;
	if (ObjectIntersect(shadowRay, tmpMaterial, shadowHitInfo, tmpDistance))
	{
		/*maths::Vector3f blue(0.0f, 0.0f, 255.0f);
		hitObject_material.set_color(blue);*/
		//return hitObject_material.color() * 0.0f;
		return false;
	}
	return true;
}



//void Raytracer::SceneGeneration(const int sphereNumber, const int sphereMaxSize, const int sphereMinSize)
//{
//	std::vector<maths::Sphere>(sphereNumber);
//	for(int i = 0; i<=sphereNumber; ++i)
//	{
//		maths::Vector3f sphereColor(rand() % 255, rand() % 255, rand() % 255);
//		Material sphereMaterial{ 0.0f,sphereColor };
//		maths::Sphere sphere((rand()%sphereMaxSize),maths::Vector3f(rand()%))
//	}


	
