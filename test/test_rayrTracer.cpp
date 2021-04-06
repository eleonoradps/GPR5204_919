
#include <gtest/gtest.h>

#include "Raytracer.h"


TEST(Raytracing, Raytracing_ImageOutput)
{
	maths::Vector3f colorTest(85.0f, 100.0f, 255.0f);
	maths::Vector3f blue(0.0f, 0.0f, 255.0f);
	maths::Vector3f red(255.0f, 0.0f, 0.0f);
	maths::Vector3f green(0.0f, 128.0f, 0.0f);

	
	Material materialTest(1.0f, red);
	Material materialTest2(1.0f, blue);
	Material materialTest3(1.0f, green);


	int width = 500;
	int heigth = 500;
	float fov = 51.52f;

	maths::Plane plane(maths::Vector3f(0.0f, -10.0f, -10.0f), maths::Vector3f(0.0f, 1.0f, 0.0f));
	plane.SetMaterial(materialTest2);
	
	maths::Sphere sphere1(6.0f, maths::Vector3f(-7.0f, 0.0f, -16.0f));
	maths::Sphere sphere3(2.0f, maths::Vector3f(4.0f, 0.0f, -8.0f));
	maths::Sphere sphere4(69.0f, maths::Vector3f(0.0f, 0.0f, -84.0f));
	/*maths::Sphere sphere4(2.0f, maths::Vector3f(6.0f, 9.0f, -15.0f));
	maths::Sphere sphere5(2.0f, maths::Vector3f(-3.0f, -4.0f, -6.0f));
	maths::Sphere sphere6(2.0f, maths::Vector3f(6.0f, 5.0f, -16.0f));
	maths::Sphere sphere7(2.0f, maths::Vector3f(3.0f, -2.0f, -7.0f));*/

	sphere1.set_material(materialTest);
	sphere3.set_material(materialTest2);
	sphere4.set_material(materialTest3);
	/*sphere5.set_material(materialTest3);
	sphere6.set_material(materialTest3);
	sphere7.set_material(materialTest3);*/

	Light light;
	std::vector<maths::Sphere> spheres;
	spheres.push_back(sphere4);
	spheres.push_back(sphere3);
	spheres.push_back(sphere1);
	/*spheres.push_back(sphere5);
	spheres.push_back(sphere6);
	spheres.push_back(sphere7);*/


	Raytracer raytracer;
	//double fov = 3.14159265358979323846 / 3;
	raytracer.SetScene(spheres, light, heigth, width, fov);
	raytracer.Render();
}

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

	Light light;
	
	Raytracer raytracer;
	raytracer.SetScene(spheres, light, heigth, width, fov);
	raytracer.Render();

	maths::Vector3f backgroundColor{ 150.0f,200.0f,255.0f };

	maths::Vector3f color = maths::Vector3f(raytracer.frameBuffer()[1].x, raytracer.frameBuffer()[1].y, raytracer.frameBuffer()[1].z);

	//Test if the first pixel (top left) is the same color as the background color. Meaning no sphere in this pixel.
	EXPECT_EQ(color.x, backgroundColor.x);

	maths::Vector3f color2 = maths::Vector3f(raytracer.frameBuffer()[125].x, raytracer.frameBuffer()[125].y, raytracer.frameBuffer()[125].z);
	//Test if the middle pixel is not the same color as the background. Meaning the sphere is in the pixel.
	EXPECT_NE(color2.x, backgroundColor.x);
}
//
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

	Light light;

	Raytracer raytracer;
	raytracer.SetScene(spheres, light, heigth, width, fov);

	maths::Vector3f rayOrigin = maths::Vector3f(0.0f, 0.0f, 0.0f);
	maths::Vector3f rayDirection = maths::Vector3f(1.0f, 0.0f, 0.0f);

	maths::Ray3 ray(rayOrigin, rayDirection);
	Material hitMaterial;
	hitInfos hit_infos;
	float distance;
	raytracer.ObjectIntersect(ray, hitMaterial, hit_infos, distance);

	maths::Vector3f expectedMaterialColor = spheres[1].material().color();
	maths::Vector3f testedMaterialColor = hitMaterial.color();
	
	//Test if the returned material color is the material from the closest sphere and not the first one in the list;
	EXPECT_EQ(expectedMaterialColor.x, testedMaterialColor.x);
}
