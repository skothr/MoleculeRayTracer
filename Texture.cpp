#include "Texture.h"
#include <GL/glew.h>

#include "Screen.h"

ShaderProgram *Texture::texSP = nullptr;
GLuint Texture::vbID = 0;

Texture::Texture()
	: size(10.0f, 10.0f)
{

}

Texture::~Texture()
{
	if(texID)
		glDeleteTextures(1, &texID);
	texID = 0;

	if(fbID)// && glCheckFramebufferStatus(fbID) == GL_FRAMEBUFFER_COMPLETE)
		glDeleteFramebuffers(1, &fbID);
	fbID = 0;
}

void Texture::create(AVec a_size, void *data)
{
	size = a_size;

	if(!texID && !fbID)
	{
		//Create texture
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);
	
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)size.x, (GLsizei)size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	
		//Create new framebuffer object
		glGenFramebuffers(1, &fbID);
		//Attach texture to framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, fbID);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texID, 0);

		//Check framebuffer
		GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if(status != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer creation has failed!\n\tStatus:  " << status << "\n\n";
	
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	//Create new texture
	//glGenTextures(1, &m_texID);

	//Set texture parameters
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)size.x, (GLsizei)size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glBindTexture(GL_TEXTURE_2D, 0);

	return;
}

GLuint Texture::getID()
{
	return texID;
}

Vec2f Texture::getSize()
{
	return size;
}

void Texture::setActive(bool active)
{
	static int last_viewport[4];
	if(active)
	{
		glBindFramebuffer(GL_FRAMEBUFFER,fbID);
		glClear(GL_COLOR_BUFFER_BIT);
		glGetIntegerv(GL_VIEWPORT, last_viewport);

		glViewport(0, 0, (int)ceil(size.x), (int)ceil(size.y));
	}
	else
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(last_viewport[0], last_viewport[1], last_viewport[2], last_viewport[3]);
	}
}

void Texture::draw(Point2f pos, Vec2f size, Vec2i view_size)
{
	Point2f glPos = toGlPoint(pos, view_size);
	Vec2f glSize = toGlVec(size, view_size);

	//glSize.y = abs(glSize.y);

	bind(this);
	
	//Load vertex array
	float data[4][4] =
	{
		{glPos.x, glPos.y, 0.0f, 0.0f},
		{glPos.x + glSize.x, glPos.y, 1.0f, 0.0f},
		{glPos.x, glPos.y + glSize.y, 0.0f, 1.0f},
		{glPos.x + glSize.x, glPos.y + glSize.y, 1.0f, 1.0f},
	};

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbID);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*4*4, data, GL_DYNAMIC_DRAW);

	glColor3f(1.0f, 1.0f, 1.0f);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);

	bind(NULL);

	return;
}

void Texture::loadResources()
{
	texSP = new ShaderProgram();
	texSP->loadShaders("shaders/textureShader.vsh", "shaders/textureShader.fsh");

	//Create vertex/index buffer objects
	float data[4][4] =
	{
		{0.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 0.0f}
	};

	glGenBuffers(1, &vbID);

	//Vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*4*4, data, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, NULL);
}

void Texture::cleanup()
{
	if(vbID)
	{
		glDeleteBuffers(1, &vbID);
		vbID = 0;
	}

	if(texSP)
	{
		delete texSP;
		texSP = nullptr;
	}
}

void Texture::bind(Texture *tex)
{
	if(tex)
	{
		//Set texture shader
		texSP->setActive();

		//Bind texture
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex->getID());

		glActiveTexture(GL_TEXTURE0);
		texSP->setUniform1i("tex", 0);
	}
	else
	{
		//Unbind texture
		glBindTexture(GL_TEXTURE_2D, NULL);
		glDisable(GL_TEXTURE_2D);

		//Set default shaders
		glUseProgram(0);
	}

	return;
}