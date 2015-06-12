#version 330

//A ray that can be traced
struct Ray
{
	vec3	origin,
			direction;
};

//Represents the values of a ray that hits an object
struct RayHit
{
	int		matIndex;
	vec3	hitPoint,
			normal;
	float	t0;
};

//Moves the ray origin slightly outside of the shape it just hit to minimize floating point errors
#define RAY_ORIGIN_SURFACE_OFFSET 0.001

//Maximum number of each object (Also set in the objects' .h files)
#define MAX_SPHERES 90
#define MAX_RECTS 8
#define MAX_MATERIALS 8
#define MAX_POINT_LIGHTS 8
#define MAX_RECT_AREA_LIGHTS 8

#define NOISE_TEX_SIZE 64

//(See .h files for a better description of the values)

//Defines a sphere
struct Sphere
{
	vec3	center;
	float	radius;
	int		material;
	int		drawOnly;
};

//Defines a rectangle 
struct Rect
{
	vec3	points[3];
	int		material;
	int		drawOnly;
};

//Defines a material that can be applied to multiple objects (referenced by its index in the materials array)
struct Material
{
	vec3	ambient,
			diffuse,
			specular,
			emissive;
	float	shininess;
};

//Defines a point light source.
struct PointLight
{
	vec3	pos,
			color;
};

//Defines a rectangular area light.
struct RectAreaLight
{
	vec3	points[3],
			color;
};

layout(location = 0) out vec4 fragColor;

//AA/AO/shadow controllers (AA and AO not implemented currently)
uniform int aaLevel;
uniform int aoSamples;
uniform int shadowDivision;

//Lists of objects passed from the cpu
uniform Material		materials[MAX_MATERIALS];
uniform Rect			rectList[MAX_RECTS];
uniform Sphere			sphereList[MAX_SPHERES];
uniform PointLight		pointLights[MAX_POINT_LIGHTS];
uniform RectAreaLight	rectAreaLights[MAX_RECT_AREA_LIGHTS];

//The number of currently active objects in each list
uniform int numMaterials;
uniform int numSpheres;
uniform int numRects;
uniform int numPointLights;
uniform int numRectAreaLights;

//If this is non-zero, the background should act as an area light.
//	(not done)
uniform int backgroundLight;

uniform sampler2D noiseTex;

smooth in vec2 texCoord;
smooth in Ray worldRay;

//Helper functions
float lerp(float v0, float v1, float t)
{
  return (1-t)*v0 + t*v1;
}

vec3 lerp(vec3 v0, vec3 v1, float t)
{
	return vec3(lerp(v0.x, v1.x, t), lerp(v0.y, v1.y, t), lerp(v0.z, v1.z, t));
}


//Intersection prototypes//

//Tests/intersects all currently implemented shapes
RayHit closestIntersection(Ray r);
bool testAnyHit(Ray r, float t_cutoff);

//Tests/intersects all spheres
float intersectSphere(Ray r, Sphere s, inout RayHit hit);
bool testSphereHit(Ray r, Sphere s, float t_cutoff);

//Tests/intersects all rects
float intersectRect(Ray r, Rect p, inout RayHit hit);
bool testRectHit(Ray r, Rect p, float t_cutoff);

void main()
{
	//Get a copy of the given world ray, and normalize just to be sure
	Ray r;
	r.origin = worldRay.origin;
	r.direction = normalize(worldRay.direction);

	//Find the RayHit object at the closest intersection point
	RayHit hit = closestIntersection(r);

	if(hit.t0 < 0.0)
	{
		//Didn't hit anything, just set background to grey
		fragColor = vec4(0.5, 0.5, 0.5, 1.0);
	}
	else if(backgroundLight != 0)
	{
		//TODO
		

	}
	else
	{
		//Lighting//

		Material mat = materials[hit.matIndex];

		vec3	diff = vec3(0.0),
				spec = vec3(0.0),

				finalColor = vec3(0.0);

		//Point lights
		for(int i = 0; i < numPointLights; i++)
		{
			//Get light
			PointLight l = pointLights[i];

			//Get vector from hit point to the light, find the length and normalize
			vec3 L = l.pos - hit.hitPoint;
			float dist = sqrt(dot(L, L));
			L /= dist;

			//Create a new ray from the hitpoint to the light to test for shadows
			Ray shadow_test;
			shadow_test.origin = hit.hitPoint;
			shadow_test.direction = L;
		
			//Multiplier that negates any light if there was something blocking it
			float blockCheck = testAnyHit(shadow_test, dist) ? 0.0 : 1.0;
			//float blockCheck = 1.0;	//<-- for no shadows

			//Calculate diffuse lighting
			diff += dot(hit.normal, L)*blockCheck;

			//Calculate specular lighting
			vec3 halfVec = normalize(normalize(-r.direction) + L);
			spec += pow(clamp(dot(hit.normal, halfVec), 0.0, 1.0), mat.shininess)*blockCheck;
		}

		//Rect area lights
		for(int i = 0; i < numRectAreaLights; i++)
		{
			RectAreaLight l = rectAreaLights[i];

			//Vectors to traverse the rectangle (from center point, which should be at index 0)
			vec3	dir1 = (l.points[1] - l.points[0])*(1.0/shadowDivision),
					dir2 = (l.points[2] - l.points[0])*(1.0/shadowDivision),

					base_p = l.points[0];// + dir1*0.5 + dir2*0.5;

			vec3	area_diff = vec3(0.0),
					area_spec = vec3(0.0);

			//vec2	noise_vec = texture(noiseTex, texCoord);
			
			vec2 noise_vec = texture(noiseTex, hit.hitPoint.xy).xz;

			//For each shadow sample, calculate the contributing light values and average.
			//	- rectangle/parallelogram is divided up into a shadowDivision by shadowDivision grid
			for(int j1 = 0; j1 < shadowDivision; j1++)
			{
				for(int j2 = 0; j2 < shadowDivision; j2++)
				{
					//Find the point on the rectangle to sample
					vec3 light_p = base_p + dir1*(float(j1) + noise_vec.x)
										  + dir2*(float(j2) + noise_vec.y);

					//Now do the same thing as with the point lights
					vec3 L = light_p - hit.hitPoint;
					float dist = sqrt(dot(L, L));
					L /= dist;

					Ray shadow_test;
					shadow_test.origin = hit.hitPoint;
					shadow_test.direction = L;

					float blockCheck = testAnyHit(shadow_test, dist) ? 0.0 : 1.0;

					//Add to the average
					area_diff += dot(hit.normal, L)*blockCheck;
					//vec3 halfVec = normalize(L - r.direction);
					//area_spec += pow(clamp(dot(hit.normal, halfVec), 0.0, 1.0), mat.shininess)*blockCheck;
				}
			}

			diff += area_diff*(1.0/(shadowDivision*shadowDivision));
			//spec += area_spec*(1.0/(shadowDivision*shadowDivision));
		}
	
		//Ambient Occlusion
		//(In progress)
		
		float visibility = 0.0;

		const float radius = 1.0;

		for(int i = 0; i < aoSamples; i++)
		{
			Ray sample;
			sample.origin = hit.hitPoint;

			vec3 tangent = cross(hit.normal, texture(noiseTex, texCoord).xyz);

			sample.direction = normalize(lerp(hit.normal, tangent, noise1(texCoord.y - i)*0.5 + 0.5));

			visibility += testAnyHit(sample, radius) ? 0.0 : 1.0;
		}
		
		visibility *= (1.0/aoSamples);
		visibility = (aoSamples <= 0) ? 1.0 : visibility;
		
		finalColor += mat.ambient + mat.diffuse*diff + mat.specular*spec + mat.emissive;

		fragColor = vec4(visibility*finalColor, 1.0);
	}
}


RayHit closestIntersection(Ray r)
{
	RayHit hit;
	hit.t0 = -1.0;
	hit.matIndex = 0;
	hit.hitPoint = vec3(0.0);
	hit.normal = vec3(0.0);

	//Check spheres
	for(int i = 0; i < numSpheres; i++)
	{
		intersectSphere(r, sphereList[i], hit);
	}

	//Check Rects
	for(int i = 0; i < numRects; i++)
	{
		intersectRect(r, rectList[i], hit);

	}

	//Check other shapes...


	hit.normal = normalize(hit.normal);
	hit.hitPoint += hit.normal*RAY_ORIGIN_SURFACE_OFFSET;

	return hit;
}

bool testAnyHit(Ray r, float t_cutoff)
{
	//Check spheres
	for(int i = 0; i < numSpheres; i++)
	{
		if(sphereList[i].drawOnly == 0 && testSphereHit(r, sphereList[i], t_cutoff))
			return true;
	}
	
	//Check rects
	for(int i = 0; i < numRects; i++)
	{
		if(rectList[i].drawOnly == 0 && testRectHit(r, rectList[i], t_cutoff))
			return true;
	}

	//Check other shapes...

	return false;
}


////SPHERE////
float intersectSphere(Ray r, Sphere s, inout RayHit hit)
{
	vec3 o = r.origin - s.center;
	float b = dot(o, r.direction);
	float c = dot(o, o) - s.radius*s.radius;

	float disc = b*b - c;

	if(disc > 0.0)
	{
		float	sdisc = sqrt(disc),
				t = -b - sdisc;

		if(t >= 0.0 && (hit.t0 < 0 || t < hit.t0))
		{
			hit.t0 = t;
			hit.hitPoint = r.origin + t*r.direction;
			hit.normal = hit.hitPoint - s.center;///s.radius;
			hit.matIndex = s.material;
		}
		//Only required for refraction.
		//else
		//{
		//	t = -b + sdisc;
		//
		//	hit.t0 = t;
		//	hit.hitPoint = r.origin + t*r.direction;
		//	hit.normal = (o + t*r.direction)/s.radius;
		//	hit.matIndex = s.material;
		//}

		return t;
	}



/*
	//vec3	origin = (s.modelMat*vec4(r.origin, 1.0)).xyz,
	//		dir = (s.modelMat*vec4(normalize(r.direction), 0.0)).xyz;
	vec3	origin = r.origin - s.center,
			dir = r.direction;

	//float rad_2 = 1.0;
	float rad_2 = s.radius*s.radius;

	vec3 r2s = -origin;
	float L_2 = dot(r2s, r2s);
	float tcaS = dot(dir, r2s);
	
	if(L_2 > rad_2 && tcaS < 0)
	{
		//MISS -- sphere is behind ray
		return -1.0;
	}
	
	float DL_2 = dot(dir, dir);
	float tca_2 = tcaS*tcaS/DL_2;
	float LM_2 = L_2 - tca_2;
	
	if(LM_2 > rad_2)
	{
		//MISS
		return -1.0;
	}

	float Lhc_2 = rad_2 - LM_2;

	if(L_2 > rad_2)
	{
		//Ray begins outside sphere
		float term1 = tcaS/DL_2;
		float term2 = sqrt(Lhc_2/DL_2);
		float t0 = term1 + term2;
		float t1 = term1 - term2;

		return (t0 < t1) ? t0 : t1;
	}
	else
	{
		return tcaS/DL_2 + sqrt(Lhc_2/DL_2);
	}
	*/
}

bool testSphereHit(Ray r, Sphere s, float t_cutoff)
{
	vec3 o = r.origin - s.center;
	float b = dot(o, r.direction);
	float c = dot(o, o) - s.radius*s.radius;

	float disc = b*b - c;

	float t;

	return (disc > 0.0 && (t = -b - sqrt(disc)) >= 0 && t <= t_cutoff);

	/*
	if(disc > 0.0)
	{
		float	sdisc = sqrt(disc),
				t = -b - sdisc;
		
		return (t >= 0.0 && t <= t_cutoff);
	}
	else
		return false;
		*/
/*
	vec3	origin = r.origin - s.center,
			dir = r.direction;

	float rad_2 = s.radius*s.radius;

	vec3 r2s = -origin;
	float L_2 = dot(r2s, r2s);
	float tcaS = dot(dir, r2s);
	
	float DL_2 = dot(dir, dir);
	float tca_2 = tcaS*tcaS/DL_2;
	float LM_2 = L_2 - tca_2;
	
	return !((L_2 > rad_2 && tcaS < 0) || LM_2 > rad_2);
	*/
}

////RECT////
float intersectRect(Ray r, Rect p, inout RayHit hit)
{
	vec3	v01 = p.points[1] - p.points[0],
			v02 = p.points[2] - p.points[0],

			n = cross(v01, v02);	//Plane normal
			
	//Find intersection with rect's plane
	float nDotDir = dot(n, r.direction);
	if(abs(nDotDir) < 0.0001)	//(almost) parallel with plane
		return -1.0;
	else
	{
		float t = dot(n, p.points[0] - r.origin)/nDotDir;

		vec3 P = r.origin + t*r.direction;
	
		vec3 other = p.points[0] + v01 + v02;	//Point in the other corner of the rect

		//Find whether it's within the rect's bounds
		mat3 basis = inverse(mat3(v01, v02, n));
	
		vec3	other_p = basis*(other - p.points[0]),
				P_p = basis*(P - p.points[0]);

		if(P_p.x >= 0.0 && P_p.y >= 0.0 && P_p.x <= other_p.x && P_p.y <= other_p.y)
		{
			if(t >= 0.0 && (hit.t0 < 0 || t < hit.t0))
			{
				hit.t0 = t;
				hit.hitPoint = r.origin + t*r.direction;
				hit.normal = n;
				hit.matIndex = p.material;
			}

			return t;
		}
		else
			return -1.0;
	}
	
}

bool testRectHit(Ray r, Rect p, float t_cutoff)
{
	RayHit temp;
	float t;
	return ((t = intersectRect(r, p, temp)) >= 0.0 && t <= t_cutoff);
}