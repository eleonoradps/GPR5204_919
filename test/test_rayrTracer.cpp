
#include <gtest/gtest.h>

#include "raytracing/ray_tracer.h"

namespace raytracing {

//Test to check if the pixel color is either the background or the sphere color
TEST(Raytracing, Color_Output)
{
	int width = 50;
	int heigth = 50;
	float fov = 51.52f;
	double bias = 1e-4;

	maths::Vector3f red(255.0f, 0.0f, 0.0f);
	Material material_test(1.0f, red);
	//Sphere is placed around the middle of the frame
	maths::Sphere sphere(50.0f, maths::Vector3f(0.0f, 0.0f, -80.0f));
	sphere.set_material(material_test);

	std::vector<maths::Sphere> spheres;
	spheres.push_back(sphere);

	PointLight light;
	std::vector<maths::Plane> planes;

	Raytracer raytracer;
	raytracer.SetScene(spheres, planes, light, heigth, width, fov, bias);
	raytracer.Render();

	maths::Vector3f background_color{ 150.0f,200.0f,255.0f };

	maths::Vector3f color = maths::Vector3f(
		raytracer.frameBuffer()[1].x, 
		raytracer.frameBuffer()[1].y, 
		raytracer.frameBuffer()[1].z);

	//Test if the first pixel (top left) is the same color as the background color. Meaning no sphere in this pixel.
	EXPECT_EQ(color.x, background_color.x);

	maths::Vector3f color2 = maths::Vector3f(
		raytracer.frameBuffer()[125].x, 
		raytracer.frameBuffer()[125].y, 
		raytracer.frameBuffer()[125].z);
	//Test if the middle pixel is not the same color as the background. Meaning the sphere is in the pixel.
	EXPECT_NE(color2.x, background_color.x);
}

// Test to check if the returned color of the ObjectIntersect method
// is the color of the closest sphere and not the color of the first one
// in the sphere list
TEST(Raytracing, Correct_Depth_Rendering)
{
	int width = 50;
	int heigth = 50;
	float fov = 51.52f;
	double bias = 1e-4;

	maths::Vector3f red(255.0f, 0.0f, 0.0f);
	maths::Vector3f green(0.0f, 128.0f, 0.0f);
	Material material_test(1.0f, red);
	Material material_test2(1.0f, green);

	//Sphere are placed around the middle of the frame, one behind the other
	maths::Sphere sphere(50.0f, maths::Vector3f(0.0f, 0.0f, -80.0f));
	maths::Sphere sphere2(50.0f, maths::Vector3f(0.0f, 0.0f, -85.0f));

	sphere.set_material(material_test);
	sphere2.set_material(material_test2);

	std::vector<maths::Sphere> spheres;
	spheres.push_back(sphere2);
	spheres.push_back(sphere);

	PointLight light;
	std::vector<maths::Plane> planes;

	Raytracer raytracer;
	raytracer.SetScene(spheres, planes, light, heigth, width, fov, bias);

	maths::Vector3f ray_origin = maths::Vector3f(0.0f, 0.0f, 0.0f);
	maths::Vector3f ray_direction = maths::Vector3f(1.0f, 0.0f, 0.0f);

	maths::Ray3 ray(ray_origin, ray_direction);
	Material hit_material;
	HitInfos hit_infos;
	float distance;
	raytracer.ObjectIntersect(ray, hit_material, hit_infos, distance);

	maths::Vector3f expected_material_color = spheres[1].material().color();
	maths::Vector3f tested_material_color = hit_material.color();

	//Test if the returned material color is the material from the closest sphere
	// and not the first one in the list;
	EXPECT_EQ(expected_material_color.x, tested_material_color.x);
}

// Test to check if the SahdowRay method correctly tell
// if a shadow ray has hit something
TEST(Raytracing, ShadowRay)
{
	int width = 50;
	int heigth = 50;
	float fov = 51.52f;
	double bias = 1e-4;

	maths::Vector3f red(255.0f, 0.0f, 0.0f);
	Material material_test(0.0f, red);

	//Sphere is placed around the middle of the frame
	maths::Sphere sphere(5.0f, maths::Vector3f(0.0f, 0.0f, 0.0f));

	PointLight light;
	//PointLight is placed at the right of the scene
	light.position = maths::Vector3f(30.0f, 0.0f, 0.0f);

	sphere.set_material(material_test);

	std::vector<maths::Sphere> spheres;
	spheres.push_back(sphere);

	std::vector<maths::Plane> planes;

	Raytracer raytracer;
	raytracer.SetScene(spheres, planes, light, heigth, width, fov, bias);

	//Shadow ray has an origin just on the left of the sphere,
	//and goes to the direction of the light, on his right
	//Return true if ray didn't hit something
	bool is_not_in_shadow = raytracer.ShadowRay(
		maths::Vector3f(-15.0f, 0.0f, 0.0f),
		maths::Vector3f(0.0f, 0.0f, 0.0f),
		maths::Vector3f(1.0f, 0.0f, 0.0f));

	EXPECT_FALSE(is_not_in_shadow);
}

// Test that will make the rendering and create a .ppn image
// of a 4 sphere scene
TEST(Raytracing, Raytracing_ImageOutput)
{
	maths::Vector3f blue(0.0f, 0.0f, 255.0f);
	maths::Vector3f red(255.0f, 0.0f, 0.0f);
	maths::Vector3f green(0.0f, 128.0f, 0.0f);
	maths::Vector3f white(255.0f, 255.0f, 0.0f);


	Material material_test(0.2f, red);
	Material material_test2(0.1f, blue);
	Material material_test3(0.0f, green);
	Material material_test4(0.0f, white);

	int width = 1920;
	int heigth = 1080;
	float fov = 51.52f;
	double bias = 1e-4;

	maths::Plane plane(maths::Vector3f(0.0f, -20.0f, 0.0f), maths::Vector3f(0.0f, 1.0f, 0.0f));
	plane.SetMaterial(material_test4);

	maths::Sphere sphere1(6.0f, maths::Vector3f(-5.0f, 0.0f, -16.0f));
	maths::Sphere sphere3(2.0f, maths::Vector3f(4.0f, 0.0f, -8.0f));
	maths::Sphere sphere4(3.0f, maths::Vector3f(8.0f, 0.0f, -5.0f));
	maths::Sphere sphere5(75.0f, maths::Vector3f(0.0f, -84.0f, -10.0f));


	sphere1.set_material(material_test);
	sphere3.set_material(material_test2);
	sphere4.set_material(material_test3);
	sphere5.set_material(material_test4);


	PointLight light;
	std::vector<maths::Sphere> spheres;
	spheres.push_back(sphere4);
	spheres.push_back(sphere3);
	spheres.push_back(sphere1);
	spheres.push_back(sphere5);

	std::vector<maths::Plane> planes;
		
	Raytracer raytracer;
	raytracer.SetScene(spheres, planes, light, heigth, width, fov, bias);
	raytracer.Render();
}
	
}// namespace raytracing
