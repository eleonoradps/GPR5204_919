
#include <gtest/gtest.h>

#include "Raytracer.h"


TEST(Raytracing, Raytracing_ImageOutput)
{
	maths::Vector3f colorTest(85.0f, 100.0f, 255.0f);
	maths::Vector3f red(255.0f, 0.0f, 0.0f);
	maths::Vector3f colorTest3(0.0f, 128.0f, 0.0f);

	
	Material materialTest(1.0f, red);
	Material materialTest2(1.0f, colorTest);
	Material materialTest3(1.0f, colorTest3);


	int width = 1920;
	int heigth = 1080;

	maths::Plane plane(maths::Vector3f(0.0f, -10.0f, -10.0f), maths::Vector3f(0.0f, 1.0f, 0.0f));
	plane.SetMaterial(materialTest2);
	
	maths::Sphere sphere1(3.0f, maths::Vector3f(-3.0f, 0.0f, -16.0f));
	maths::Sphere sphere3(2.0f, maths::Vector3f(-3.0f, 0.0f, -10.0f));
	/*maths::Sphere sphere4(2.0f, maths::Vector3f(6.0f, 9.0f, -20.0f));
	maths::Sphere sphere5(2.0f, maths::Vector3f(-3.0f, -4.0f, -6.0f));
	maths::Sphere sphere6(2.0f, maths::Vector3f(6.0f, 5.0f, -16.0f));
	maths::Sphere sphere7(2.0f, maths::Vector3f(3.0f, -2.0f, -7.0f));*/

	sphere1.set_material(materialTest);
	sphere3.set_material(materialTest3);
	/*sphere4.set_material(materialTest3);
	sphere5.set_material(materialTest3);
	sphere6.set_material(materialTest3);
	sphere7.set_material(materialTest3);*/

	Light light;
	std::vector<maths::Sphere> spheres;
	spheres.push_back(sphere1);
	spheres.push_back(sphere3);
	/*spheres.push_back(sphere4);
	spheres.push_back(sphere5);
	spheres.push_back(sphere6);
	spheres.push_back(sphere7);*/


	Raytracer raytracer;
	//double fov = 3.14159265358979323846 / 3;
	float fov = 51.52f;

	raytracer.Render(width, heigth, fov, spheres, plane, light);
}