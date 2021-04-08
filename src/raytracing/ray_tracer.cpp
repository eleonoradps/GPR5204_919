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

#include <omp.h>
#include <fstream>

#include "raytracing/ray_tracer.h"

namespace raytracing {

inline
	float clamp(float& rgb_value)
{
	if (rgb_value > 255.0f)
	{
		rgb_value = 255.0f;
	}
	if (rgb_value < 0.0f)
	{
		rgb_value = 0.0f;
	}
	return rgb_value;
	}

bool Raytracer::ObjectIntersect(
	maths::Ray3& ray,
	Material& hit_material, 
	HitInfos& hit_info, 
	float& distance)
{
	float max_distance = 1000000.0f;
	distance = max_distance;
	for (int i = 0; i < spheres_.size(); ++i)
	{
		hit_info.distance = max_distance;
		// Only render intersection for the nearest sphere,
		// if distance is smaller than previously
		if (ray.IntersectSphere(spheres_[i], hit_info.hit_position, hit_info.distance)
			&& hit_info.distance < distance)
		{
			//Set hit info value regarding the object that was hit
			hit_info.normal = maths::Vector3f(hit_info.hit_position - spheres_[i].center()).Normalized();
			hit_material = spheres_[i].material();
			distance = hit_info.distance;
		}
	}
	if (distance != max_distance)
	{
		// Means that the ray had an intersection
		return true;
	}

	if (planes_.size() != 0)
	{
		for (int i = 0; i < planes_.size(); ++i)
		{
			if (ray.IntersectPlane(planes_[i], hit_info.hit_position))
			{
				hit_info.normal = planes_[i].normal();
				hit_material = planes_[i].material();

				return true;
			}
		}
	}
	return false;
}

maths::Vector3f Raytracer::RayCast(
	const maths::Vector3f& origin,
	const maths::Vector3f& ray_direction, 
	const int& depth)
{
	maths::Ray3 ray{ origin, ray_direction };
	Material hit_object_material;
	HitInfos hit_info;
	float distance;
	bool in_light = true;

	//If the ray didn't hit anything or if the recursive depth of the raycasting
	// is greater than 4, return background color
	if (depth > 4 || !ObjectIntersect(ray, hit_object_material, hit_info, distance))
	{
		return background_color_;
	}
	else
	{
		//Compute the normal or direction of the light
		maths::Vector3f light_normal(light_.position - hit_info.hit_position);
		light_normal.Normalize();

		//Compute shadow ray to check if point is in shadow
		in_light = ShadowRay(hit_info.hit_position, hit_info.normal, light_normal);

		// Calculate how much light is the point getting
		float light_value = maths::Vector3f::Dot(hit_info.normal, light_normal);
		if (light_value < 0.0f)
		{
			light_value = 0.0f;
		}

		if (in_light)
		{
			// Point is not in the shadow
			// Cast reflexion ray recursively to compute reflexion color
			const maths::Vector3f reflection_direction = Reflect(ray_direction, hit_info.normal).Normalized();
			const maths::Vector3f reflection_origin(hit_info.hit_position + hit_info.normal * bias_);
			const maths::Vector3f reflection_color = RayCast(reflection_origin, reflection_direction, depth + 1);
			hit_object_material.set_color(hit_object_material.color() * light_value 
				+= reflection_color * hit_object_material.reflexion_index());
		}
		else
		{
			//Point is in the shadow
			hit_object_material.set_color(hit_object_material.color() * light_value * in_light);
		}
		return (hit_object_material.color());
	}
}

void Raytracer::Render()
{
#pragma omp parallel for
	for (int i = 0; i < height_; ++i)
	{
		for (int j = 0; j < width_; ++j)
		{
			double dir_x = (j + 0.5f) - width_ / 2.0;
			double dir_y = -(i + 0.5f) + height_ / 2.0;
			double dir_z = -height_ / (2.0 * tan(fov_ / 2.0));

			maths::Vector3f ray_direction = maths::Vector3f(dir_x, dir_y, dir_z).Normalized();

			frame_buffer_[j + i * width_] = RayCast(maths::Vector3f(0.0f, 0.0f, 0.0f),
				ray_direction);
		}
	}
	WriteImage();
}

void Raytracer::WriteImage()
{
	std::ofstream ofs("./image.ppm", std::ios::out | std::ios::binary);
	ofs << "P6\n" << width_ << " " << height_ << "\n255\n";
	for (uint32_t i = 0; i < height_ * width_; ++i) {
		char r = (char)(clamp(frame_buffer_[i].x));
		char g = (char)(clamp(frame_buffer_[i].y));
		char b = (char)(clamp(frame_buffer_[i].z));

		ofs << r << g << b;
	}
	ofs.close();
}

bool Raytracer::ShadowRay(
	const maths::Vector3f& hit_position, 
	const maths::Vector3f& hit_normal, 
	const maths::Vector3f& light_normal)
{
	//Add a bias along the normal to prevent self collision
	const maths::Vector3f shadow_ray_origin(hit_position + hit_normal * bias_);
	maths::Ray3 shadow_ray(shadow_ray_origin, light_normal);
	Material tmp_material;
	HitInfos shadow_hit_info;
	float tmp_distance;
		
	if (ObjectIntersect(shadow_ray, tmp_material, shadow_hit_info, tmp_distance))
	{
		// Return that the point is in the shadow
		return false;
	}
	// Return that the point is in the light
	return true;
}

maths::Vector3f Raytracer::Reflect(
	const maths::Vector3f& ray_direction, 
	const maths::Vector3f& hit_normal)
{
	const float t = maths::Vector3f::Dot(ray_direction, hit_normal);
	const float r = 2.0f * t;
	return ray_direction - hit_normal * r;
}
	
}// namespace raytracing



	
