#include "RayTracer.h"
#include "RayObject.h"
#include "SphereObject.h"
#include "RectObject.h"
#include "Material.h"
#include "PointLight.h"
#include "RectAreaLight.h"

#include <GL/glew.h>


/////RAY TRACER/////

const GuiPropFlags RayTracer::PROP_FLAGS = PFlags::HARD_BACK;

const vec2 RayTracer::quadVertices[4] =		{	vec2(-1.0f, -1.0f),
												vec2(1.0f, -1.0f),
												vec2(1.0f, 1.0f),
												vec2(-1.0f, 1.0f)	};

const GLuint RayTracer::quadIndices[6] = { 0, 1, 2, 0, 2, 3 };

const int					RayTracer::AO_SAMPLES = 0,
							RayTracer::SHADOW_DIVISION = 4,	//SHADOW_DIVISION*SHADOW_DIVISION total samples (divided along 2 dimensions)
							RayTracer::NOISE_TEX_SIZE = 64;

vec4 noiseData[RayTracer::NOISE_TEX_SIZE][RayTracer::NOISE_TEX_SIZE];

RayTracer::RayTracer(ParentElement *parent_, APoint a_pos, AVec a_size, GuiStateFlags s_flags)
	: GuiElement(parent_, a_pos, a_size, GuiProps(s_flags, PROP_FLAGS)),
		Control(GuiProps(s_flags, PROP_FLAGS)),
		renderCamera(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f), Vec3f(0.0f, 1.0f, 0.0f))
{
	renderCamera.setProj(45.0f, a_size.x/a_size.y, 0.1f, 100.0f);
	init();
}

RayTracer::~RayTracer()
{
	if(quad_vaoID)
	{
		glDeleteVertexArrays(1, &quad_vaoID);
		glDeleteBuffers(1, &quad_vboID);
		glDeleteBuffers(1, &quad_indID);
	}
}

void RayTracer::init()
{
	rayProgram.loadShaders("raytrace.vsh", "raytrace.fsh");

	//Set constant shader uniforms
	rayProgram.setUniform1i("noiseTex", 2);

	//Init quad vao
	glGenVertexArrays(1, &quad_vaoID);
	glGenBuffers(1, &quad_vboID);
	glGenBuffers(1, &quad_indID);

	glBindVertexArray(quad_vaoID);
		glBindBuffer(GL_ARRAY_BUFFER, quad_vboID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices[0].x, GL_STATIC_DRAW);
		
		//GL_CHECK_ERRORS
		std::cout << gluErrorString(glGetError()) << "\n";

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad_indID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), &quadIndices[0], GL_STATIC_DRAW);
		
		std::cout << gluErrorString(glGetError()) << "\n";
		
	//Init noise texture
	for(int j = 0; j < NOISE_TEX_SIZE; j++)
	{
		for(int i = 0; i < NOISE_TEX_SIZE; i++)
		{
			vec4 &d = noiseData[i][j];
			d.x = ((float)rand() / RAND_MAX);
			d.y = ((float)rand() / RAND_MAX);
			d.z = 0.0f;//(float)rand() / RAND_MAX;
			d.w = 0.0f;//(float)rand() / RAND_MAX;

			float len = sqrt(d.x*d.x + d.y*d.y);
			d.x /= len;
			d.y /= len;
			//noiseData[i][j].z /= len;
			d.x = d.x*0.5f + 0.5f;
			d.y = d.y*0.5f + 0.5f;
			d.z = d.z*0.5f + 0.5f;

		}
	}
	
	glGenTextures(1, &noise_texID);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, noise_texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, NOISE_TEX_SIZE, NOISE_TEX_SIZE, 0, GL_BGRA, GL_FLOAT, noiseData);


	setShadows(true);
	setAntialiasing(AALevel::AA1X);
	setAmbientOcclusion(true);
}

void RayTracer::onDrag(APoint m_pos, AVec d_pos, bool direct)
{
	if(clickSelect)
	{
		renderCamera.rotate(d_pos.y*(1.0f/400.0f), d_pos.x*(1.0f/400.0f), 0.0f);
	}
}

void RayTracer::onKeyDown(KeyCode key)
{
	switch(key)
	{
	case KeyCode::K_W:
		forwardMoveMult += 1.0f;
		break;
	case KeyCode::K_A:
		rightMoveMult -= 1.0f;
		break;
	case KeyCode::K_S:
		forwardMoveMult -= 1.0f;
		break;
	case KeyCode::K_D:
		rightMoveMult += 1.0f;
		break;
	case KeyCode::K_SPACE:
		upMoveMult += 1.0;
		break;
	case KeyCode::K_LCTRL:
	case KeyCode::K_RCTRL:
		upMoveMult -= 1.0f;
		break;
	}
}

void RayTracer::onKeyUp(KeyCode key)
{
	switch(key)
	{
	case KeyCode::K_W:
		forwardMoveMult -= 1.0f;
		break;
	case KeyCode::K_A:
		rightMoveMult += 1.0f;
		break;
	case KeyCode::K_S:
		forwardMoveMult += 1.0f;
		break;
	case KeyCode::K_D:
		rightMoveMult -= 1.0f;
		break;
	case KeyCode::K_SPACE:
		upMoveMult -= 1.0;
		break;
	case KeyCode::K_LCTRL:
	case KeyCode::K_RCTRL:
		upMoveMult += 1.0f;
		break;
	}
}

void RayTracer::onSizeChanged(AVec d_size)
{
	Control::onSizeChanged(d_size);
	renderCamera.setAspect(size.x/size.y);
}

/*
void RayTracer::setCamera(Camera *new_camera)
{
	renderCamera = new_camera;
}
*/

/*
void RayTracer::addObjects(const std::vector<RayObject*> &objs)
{
	objects.reserve(objects.size() + objs.size());
	objects.insert(objs.begin(), objs.end());

	SphereObject::updateShaderList(&rayProgram);

	
	//for(auto o : objs)
	//{
	//	o->updateShader(&rayProgram);
	//	objects.emplace(o);
	//}
	
}
*/
void RayTracer::updateObjects()
{
	SphereObject::updateShaderList(&rayProgram);
	RectObject::updateShaderList(&rayProgram);
}

void RayTracer::updateMaterials()
{
	Material::updateShaderList(&rayProgram);
}

void RayTracer::updateLights()
{
	PointLight::updateShaderList(&rayProgram);
	RectAreaLight::updateShaderList(&rayProgram);
}

void RayTracer::setAntialiasing(AALevel aa)
{
	if(aa != antialiasing)
	{
		antialiasing = aa;
		rayProgram.setUniform1i("aaLevel", (int)aa);
	}
}

void RayTracer::setAmbientOcclusion(bool on)
{
	if(on != ambientOcclusion)
	{
		ambientOcclusion = on;
		rayProgram.setUniform1i("aoSamples", ambientOcclusion ? AO_SAMPLES : 0);
	}
}

void RayTracer::setShadows(bool on)
{
	if(on != shadows)
	{
		shadows = on;
		rayProgram.setUniform1i("shadowDivision", shadows ? SHADOW_DIVISION : 0);
	}
}

void RayTracer::update(const Time &dt)
{
	float speed = 20.0f;
	
	renderCamera.setPos(renderCamera.getPos()
						+ (	renderCamera.getEyeDir()*forwardMoveMult
							+ renderCamera.getRightDir()*rightMoveMult
							+ renderCamera.getUpDir()*upMoveMult	)*speed*dt);

	renderCamera.update();
}

void RayTracer::draw(GlInterface &gl)
{
	if(isolateViewport(gl))
	{
		Vec2i view_size = gl.getCurrView().size;
		
		rayProgram.setActive();

		glBindVertexArray(quad_vaoID);
			Point3f cam_pos = renderCamera.getPos();
			Vec3f eye_dir = renderCamera.getEyeDir(),
					right_dir = renderCamera.getRightDir(),
					up_dir = renderCamera.getUpDir();

			//Set volatile shader uniforms
			rayProgram.setUniformMat4("viewMatrix", renderCamera.getView());
			rayProgram.setUniformMat4("invProjMatrix", renderCamera.getProj().getInverse());
			
			rayProgram.setUniform3f("camPos", cam_pos.x, cam_pos.y, cam_pos.z);

			/*
			rayProgram.setUniform2i("screenSize", view_size.x, view_size.y);

			rayProgram.setUniform3f("camEyeDir", eye_dir.x, eye_dir.y, eye_dir.z);
			rayProgram.setUniform3f("camRightDir", right_dir.x, right_dir.y, right_dir.z);
			rayProgram.setUniform3f("camUpDir", up_dir.x, up_dir.y, up_dir.z);

			rayProgram.setUniform1f("camNear", renderCamera.getNearZ());
			rayProgram.setUniform1f("camFar", renderCamera.getFarZ());
			rayProgram.setUniform1f("camFov", renderCamera.getFOV());
			rayProgram.setUniform1f("camAspect", renderCamera.getAspect());
			*/

			//Draw quad (and raytrace)
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);

		restoreViewport(gl);
	}
}