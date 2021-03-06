#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "Shaders.h"
#include "Control.h"
#include "Camera.h"

#include <unordered_set>

//Tiny vector classes for passing arrays of vectors to the shader
//	(Something was off with the stride of my templated vector classes, I need to rewrite those)
struct vec2
{
	float x, y;

	vec2()
		: x(0.0f), y(0.0f)
	{ }

	vec2(float x_, float y_)
		: x(x_), y(y_)
	{ }
};

struct vec4
{
	float x, y, z, w;

	vec4()
		: x(0.0f), y(0.0f), z(0.0f), w(0.0f)
	{ }

	vec4(float x_, float y_, float z_, float w_)
		: x(x_), y(y_), z(z_), w(w_)
	{ }
};

//Defines different antialiasing levels (each pixel subdivided by that amount in each dimension)
//	(currently not implemented in the shader, just sets the uniform)
enum class AntialiasingLevel
{
	INVALID = 0,

	AA1X = 1,
	AA2X = 2,
	AA4X = 4,
	AA8X = 8,
	AA16X = 16,
	AA32X = 32,
};
typedef AntialiasingLevel AALevel;



class RayObject;


//Main class for raytracing. 
//	- Holds the main raytracing shader program, and has a full-screen quad VAO/VBO and a noise texture, all of which are passed to the shader.
//	- Also owns the camera, and controls its movement/rotation with calls from the ActiveElement GUI base class.
//	- Currently, all RayObjects are statically linked to this class, and their resources can all be sent to the shader
//		with calls to updateObjects(), updateMaterials(), and updateLights().
//
//
//	- See the README.md for info on how to port it to a non-windows os, or another window wrapper library in case the GUI doesn't work.
//
class RayTracer : public Control
{
private:
	ShaderProgram rayProgram;

	GLuint	quad_vaoID = 0,
			quad_vboID = 0,
			quad_indID = 0,

			noise_texID = 0;

	AALevel antialiasing = AALevel::INVALID;
	bool	ambientOcclusion = false,
			shadows = false;

	//std::unordered_set<RayObject*> objects;
	//std::unordered_set<Materials

	static const vec2 quadVertices[4];
	static const GLuint quadIndices[6];

	float	rightMoveMult = 0.0f,
			forwardMoveMult = 0.0f,
			upMoveMult = 0.0f;

	Camera renderCamera;

	void init();

	virtual void onDrag(APoint m_pos, AVec d_pos, bool direct) override;

	virtual void onKeyDown(KeyCode key) override;
	virtual void onKeyUp(KeyCode key) override;

	virtual void onSizeChanged(AVec d_size) override;

public:
	RayTracer(ParentElement *parent_, APoint a_pos, AVec a_size, GuiStateFlags s_flags);
	virtual ~RayTracer();

	static const GuiPropFlags	PROP_FLAGS;
	static const int			AO_SAMPLES,
								SHADOW_DIVISION,
								NOISE_TEX_SIZE;

	//void addObjects(const std::vector<RayObject*> &objs);
	//void addMaterials(const std::vector<Material*> &mats);

	void updateObjects();
	void updateMaterials();
	void updateLights();

	void setAntialiasing(AALevel aa);
	void setAmbientOcclusion(bool on);
	void setShadows(bool on);

	//void setCamera(Camera *new_camera);

	virtual void update(const Time &dt) override;
	virtual void draw(GlInterface &gl) override;
};

#endif	//RAYTRACER_H