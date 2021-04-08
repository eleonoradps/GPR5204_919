
#include <gtest/gtest.h>

#include "Raytracer.h"

//Test to check if the pixel color is either the background or the sphere color
TEST(Raytracing, Color_Output)
{
	int width = 50;
	int heigth = 50;
	float fov = 51.52f;
	
	maths::Vector3f red(255.0f, 0.0f, 0.0f);
	Material materialTest(1.0f, red);
	//Sphere is placed around the middle of the frame
	maths::Sphere sphere(50.0f, maths::Vector3f(0.0f, 0.0f, -80.0f));
	sphere.set_material(materialTest);

	std::vector<maths::Sphere> spheres;
	spheres.push_back(sphere);

	PointLight light;
	std::vector<maths::Plane> planes;

	
	Raytracer raytracer;
	raytracer.SetScene(spheres, planes, light, heigth, width, fov);
	raytracer.Render();

	maths::Vector3f backgroundColor{ 150.0f,200.0f,255.0f };

	maths::Vector3f color = maths::Vector3f(raytracer.frameBuffer()[1].x, raytracer.frameBuffer()[1].y, raytracer.frameBuffer()[1].z);

	//Test if the first pixel (top left) is the same color as the background color. Meaning no sphere in this pixel.
	EXPECT_EQ(color.x, backgroundColor.x);

	maths::Vector3f color2 = maths::Vector3f(raytracer.frameBuffer()[125].x, raytracer.frameBuffer()[125].y, raytracer.frameBuffer()[125].z);
	//Test if the middle pixel is not the same color as the background. Meaning the sphere is in the pixel.
	EXPECT_NE(color2.x, backgroundColor.x);
}

// Test to check if the returned color of the ObjectIntersect method
// is the color of the closest sphere and not the color of the first one
// in the sphere list
TEST(Raytracing, Correct_Depth_Rendering)
{
	int width = 50;
	int heigth = 50;
	float fov = 51.52f;

	maths::Vector3f red(255.0f, 0.0f, 0.0f);
	maths::Vector3f green(0.0f, 128.0f, 0.0f);
	Material materialTest(1.0f, red);
	Material materialTest2(1.0f, green);

	//Sphere are placed around the middle of the frame, one behind the other
	maths::Sphere sphere(50.0f, maths::Vector3f(0.0f, 0.0f, -80.0f));
	maths::Sphere sphere2(50.0f, maths::Vector3f(0.0f, 0.0f, -85.0f));

	sphere.set_material(materialTest);
	sphere2.set_material(materialTest2);

	std::vector<maths::Sphere> spheres;
	spheres.push_back(sphere2);
	spheres.push_back(sphere);

	PointLight light;
	std::vector<maths::Plane> planes;


	Raytracer raytracer;
	raytracer.SetScene(spheres, planes, light, heigth, width, fov);

	maths::Vector3f rayOrigin = maths::Vector3f(0.0f, 0.0f, 0.0f);
	maths::Vector3f rayDirection = maths::Vector3f(1.0f, 0.0f, 0.0f);

	maths::Ray3 ray(rayOrigin, rayDirection);
	Material hitMaterial;
	hitInfos hit_infos;
	float distance;
	raytracer.ObjectIntersect(ray, hitMaterial, hit_infos, distance);

	maths::Vector3f expectedMaterialColor = spheres[1].material().color();
	maths::Vector3f testedMaterialColor = hitMaterial.color();
	
	//Test if the returned material color is the material from the closest sphere
	// and not the first one in the list;
	EXPECT_EQ(expectedMaterialColor.x, testedMaterialColor.x);
}

// Test to check if the SahdowRay method correctly tell
// if a shadow ray has hit something
TEST(Raytracing, ShadowRay)
{
	int width = 50;
	int heigth = 50;
	float fov = 51.52f;

	maths::Vector3f red(255.0f, 0.0f, 0.0f);
	Material materialTest(0.0f, red);

	//Sphere is placed around the middle of the frame
	maths::Sphere sphere(5.0f, maths::Vector3f(0.0f, 0.0f, 0.0f));

	PointLight light;
	//PointLight is placed at the right of the scene
	light.position = maths::Vector3f(30.0f, 0.0f, 0.0f);

	sphere.set_material(materialTest);

	std::vector<maths::Sphere> spheres;
	spheres.push_back(sphere);

	std::vector<maths::Plane> planes;

	Raytracer raytracer;
	raytracer.SetScene(spheres, planes,light, heigth, width, fov);

	//Shadow ray has an origin just on the left of the sphere,
	//and goes to the direction of the light, on his right
	//Return true if ray didn't hit something
	bool isNotInShadow = raytracer.ShadowRay(
		maths::Vector3f(-15.0f, 0.0f, 0.0f),
		maths::Vector3f(0.0f, 0.0f, 0.0f),
		maths::Vector3f(1.0f, 0.0f, 0.0f));

	EXPECT_FALSE(isNotInShadow);
}

// Test that will make the rendering and create a .ppn image
// of a 3 sphere scene
TEST(Raytracing, Raytracing_ImageOutput)
{
	maths::Vector3f colorTest(85.0f, 100.0f, 255.0f);
	maths::Vector3f blue(0.0f, 0.0f, 255.0f);
	maths::Vector3f red(255.0f, 0.0f, 0.0f);
	maths::Vector3f green(0.0f, 128.0f, 0.0f);
	maths::Vector3f white(255.0f, 255.0f, 0.0f);


	Material materialTest(0.1f, red);
	Material materialTest2(0.1f, blue);
	Material materialTest3(0.0f, green);
	Material materialTest4(0.0f, white);


	int width = 1920;
	int heigth = 1080;
	float fov = 51.52f;

	maths::Plane plane(maths::Vector3f(0.0f, -20.0f, 0.0f), maths::Vector3f(0.0f, 1.0f, 0.0f));
	plane.SetMaterial(materialTest4);

	maths::Sphere sphere1(6.0f, maths::Vector3f(-5.0f, 0.0f, -16.0f));
	maths::Sphere sphere3(2.0f, maths::Vector3f(4.0f, 0.0f, -8.0f));
	maths::Sphere sphere4(3.0f, maths::Vector3f(8.0f, 0.0f, -5.0f));
	maths::Sphere sphere5(75.0f, maths::Vector3f(0.0f, -84.0f, -10.0f));


	sphere1.set_material(materialTest);
	sphere3.set_material(materialTest2);
	sphere4.set_material(materialTest3);
	sphere5.set_material(materialTest4);


	PointLight light;
	std::vector<maths::Sphere> spheres;
	spheres.push_back(sphere4);
	spheres.push_back(sphere3);
	spheres.push_back(sphere1);
	spheres.push_back(sphere5);

	std::vector<maths::Plane> planes;
	//planes.push_back(plane);
	
	Raytracer raytracer;
	raytracer.SetScene(spheres, planes,light, heigth, width, fov);
	raytracer.Render();
}
