#version 330

struct Ray
{
	vec3	origin,
			direction;
};

layout(location = 0) in vec2 vertex;


uniform vec3	camPos;

/*
				camEyeDir,
				camRightDir,
				camUpDir;
				
uniform ivec2	screenSize;

uniform float	camNear,
				camFar,
				camFov,
				camAspect;
				*/
				
uniform mat4 viewMatrix;
uniform mat4 invProjMatrix;

smooth out vec2 texCoord;
smooth out Ray worldRay;

void main()
{
	//-1 -> 1
	gl_Position = vec4(vertex, 0.0, 1.0);

	//0 --> 1
	texCoord = vertex*0.5 + 0.5;
	
	//Rays start at the camera's position
	worldRay.origin = camPos;

	//Transform vertices into view space with the inverse projection matrix, and then into world space with the view matrix.
	worldRay.direction = (invProjMatrix*vec4(vertex, 0.0, 1.0)*viewMatrix).xyz;
	worldRay.direction = normalize(worldRay.direction);
}
