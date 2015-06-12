
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
	//Initialize GUI
	initApollo();

	Window		*window = new Window("Molecular Viewer", APoint(300, 250), AVec(1280, 720), DEFAULT_STATE);
	
	//Create ray tracer object and connect to the window
	RayTracer	rt(window, APoint(0.0f, 0.0f), AVec(1280.0f, 720.0f), DEFAULT_STATE);

	//Attach to the edges of the window (in case it's resized)
	rt.attachTo(window, Side::LEFT, 0);
	rt.attachTo(window, Side::RIGHT, 0);
	rt.attachTo(window, Side::TOP, 0);
	rt.attachTo(window, Side::BOTTOM, 0);

	//Create materials
	Material		m0(Color(0.0f, 0.0f, 0.05f), Color(0.2f, 0.2f, 0.8f), Color(0.1f, 0.1f, 0.2f), Color(0.0f, 0.0f, 0.0f), 50.0f),
					m1(Color(0.05f, 0.0f, 0.0f), Color(0.8f, 0.2f, 0.15f), Color(0.4f, 0.2f, 0.3f), Color(0.0f, 0.0f, 0.0f), 10.0f);

	//Create random spheres
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

	//Create light source
	//PointLight		l0(Point3f(0.0f, 20.0f, 0.0f), Color(0.5f, 0.5f, 0.5f));
	RectAreaLight		l1(Point3f(3.0f, 20.0f, 3.0f), Point3f(-3.0f, 20.0f, 3.0f), Point3f(3.0f, 20.0f, -3.0f), Color(0.5f, 0.5f, 0.5f));
	
	//Send states to the raytracing shader
	rt.updateObjects();
	rt.updateLights();
	rt.updateMaterials();
	
	HRes_Clock timer(true);

	//Main window loop
	while(window->isOpen())
	{
		if(Keyboard::keyDown(KeyCode::K_ESCAPE))
		{
			window->close();
			break;
		}

		//Handle window events
		window->handleWindow();

		//Update physics
		timer.nextTimeFrame();
		window->update(timer.dt);

		//Render
		window->draw();
	}

	//Clean up
	for(auto ss : s)
		delete ss;
	s.clear();

	cleanupApollo();

	return 0;
}