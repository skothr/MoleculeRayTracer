#ifndef APOLLO_TEXTURE_H
#define APOLLO_TEXTURE_H

#include "Shaders.h"

class Texture
{
private:
	static ShaderProgram *texSP;
	static GLuint vbID;

	GLuint	texID = 0,
			fbID = 0;
	Vec2f	size;


public:
	Texture();
	//Texture(const Texture &other);
	virtual ~Texture();

	Texture& operator=(const Texture &other);

	void create(Vec2f a_size, void *data = nullptr);

	static void loadResources();
	static void cleanup();
	static void bind(Texture *tex);

	GLuint getID();
	
	Vec2f getSize();

	void setActive(bool active);	//Draws to this texture
	void draw(Point2f a_pos, Vec2f a_size, Vec2i view_size);
};

#endif