
#include "Apollo.h"
#include "Camera.h"
#include "Timing.h"

#include "RayTracer.h"
#include "SphereObject.h"
#include "Material.h"
#include "PointLight.h"
#include "RectAreaLight.h"


#include "Color.h"

#include <vector>

int main()
{
	initApollo();

	Window		*window = new Window("Molecular Viewer", APoint(300, 250), AVec(1280, 720), DEFAULT_STATE);
	
	//Camera		camera(Point3f(0.0f, 2.0f, 5.0f), Vec3f(0.0f, 0.0f, -1.0f), Vec3f(0.0f, 1.0f, 0.0f));
	RayTracer	rt(window, APoint(0.0f, 0.0f), AVec(1280.0f, 720.0f), DEFAULT_STATE);

	rt.attachTo(window, Side::LEFT, 0);
	rt.attachTo(window, Side::RIGHT, 0);
	rt.attachTo(window, Side::TOP, 0);
	rt.attachTo(window, Side::BOTTOM, 0);
	
	rt.setShadows(true);

	Material		m0(Color(0.0f, 0.0f, 0.05f), Color(0.2f, 0.2f, 0.8f), Color(0.1f, 0.1f, 0.2f), Color(0.0f, 0.0f, 0.0f), 50.0f),
					m1(Color(0.05f, 0.0f, 0.0f), Color(0.8f, 0.2f, 0.15f), Color(0.4f, 0.2f, 0.3f), Color(0.0f, 0.0f, 0.0f), 10.0f);


	const int NUM_SPHERES = 89;
	std::vector<SphereObject*> s;
	s.reserve(NUM_SPHERES);

	for(int i = 0; i < NUM_SPHERES; i++)
	{
		float	x = ((float)rand()/(float)RAND_MAX)*2.0f - 1.0f,
				y = ((float)rand()/(float)RAND_MAX),
				z = ((float)rand()/(float)RAND_MAX)*2.0f - 1.0f;

		//std::cout << x << ", " << y << ", " << z << "\n";

		s.push_back(new SphereObject(Point3f(x*10.0f, y*10.0f, z*10.0f), 1.0f, (rand() % 2 ? &m0 : &m1)));
	}

	//SphereObject	s0(Point3f(0.0f, 0.0f, 0.0f), 1.0f, &m0),
	//				s1(Point3f(2.0f, 2.0f, 0.0f), 1.0f, &m1),
	//				s2(Point3f(4.0f, 4.0f, 0.0f), 1.0f, &m0),
	//				s3(Point3f(6.0f, 6.0f, 0.0f), 1.0f, &m1),
	//				s4(Point3f(8.0f, 8.0f, 0.0f), 1.0f, &m0),
	//				s5(Point3f(10.0f, 10.0f, 0.0f), 1.0f, &m1),
	//				s6(Point3f(12.0f, 12.0f, 0.0f), 1.0f, &m0);

	//RectObject		r0(Point3f(2.0f, 3.0f, 0.0f), Point3f(-2.0f, 3.0f, 0.0f), Point3f(2.0f, 6.0f, 0.0f), &Material::LIGHT_MATERIAL);


	//PointLight		l0(Point3f(0.0f, 20.0f, 0.0f), Color(0.5f, 0.5f, 0.5f));
	RectAreaLight		l1(Point3f(3.0f, 20.0f, 3.0f), Point3f(-3.0f, 20.0f, 3.0f), Point3f(3.0f, 20.0f, -3.0f), Color(0.5f, 0.5f, 0.5f));
	
	rt.updateObjects();
	rt.updateLights();
	rt.updateMaterials();

	//rt.addObjects({&s0, &s1, &s2, &s3, &s4, &s5, &s6});
	
	HRes_Clock timer(true);

	while(window->isOpen())
	{
		if(Keyboard::keyDown(KeyCode::K_ESCAPE))
		{
			window->close();
			break;
		}

		window->handleWindow();

		timer.nextTimeFrame();
		window->update(timer.dt);

		window->draw();
	}

	for(auto ss : s)
		delete ss;
	s.clear();

	return 0;
}