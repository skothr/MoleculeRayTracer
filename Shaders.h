#ifndef SHADERS_H
#define SHADERS_H

#include <string>
#include <vector>

#include "Vector.h"
#include "Matrix.h"

typedef unsigned int GLenum;
typedef unsigned int GLuint;
typedef char GLchar;

class Shader
{
private:
	GLenum shaderType;
	GLuint shaderID;
	
	std::string pathName;
	std::string fileName;

	GLchar	*src;
	std::string srcCode;
	
	bool compile();
	bool load(GLuint programID);

	bool readShaderFile();

public:
	Shader();
	~Shader();

	void vBareShader(GLuint programID);
	void fBareShader(GLuint programID);
	
	void setPath(const char *pPath);
	bool loadFromFile(GLuint programID, std::string name, GLenum type);

	void print();

	GLuint getShaderID();

};


class ShaderProgram
{
private:
	GLuint programID;
	Shader vShader, fShader;

	std::vector<GLuint> uniformLocations;
	std::vector<std::string> uniformNames;

	std::vector<GLuint> attributeLocations;
	std::vector<std::string> attributeNames;
	
	bool linkProgram();

	void getUniformLocations();

	int getUniformIndex(std::string name);

public:
	ShaderProgram();
	ShaderProgram(const char* argv_0);
	~ShaderProgram();

	static bool canRunShaders();
	
	bool loadShaders(std::string vShaderFile, std::string fShaderFile);

	void setActive();

	GLuint getProgramID();
	GLuint getVShaderID();
	GLuint getFShaderID();

	//Functions to set uniforms
	bool setUniform1f(std::string name, float v0);
	bool setUniform2f(std::string name, float v0, float v1);
	bool setUniform3f(std::string name, float v0, float v1, float v2);
	bool setUniform4f(std::string name, float v0, float v1, float v2, float v3);

	bool setUniform1i(std::string name, int v0);
	bool setUniform2i(std::string name, int v0, int v1);
	bool setUniform3i(std::string name, int v0, int v1, int v2);
	bool setUniform4i(std::string name, int v0, int v1, int v2, int v3);
	
	bool setUniform1fv(std::string name, int count, const float *v);
	bool setUniform2fv(std::string name, int count, const float *v);
	bool setUniform3fv(std::string name, int count, const float *v);
	bool setUniform4fv(std::string name, int count, const float *v);

	bool setUniform1iv(std::string name, int count, const int *v);
	bool setUniform2iv(std::string name, int count, const int *v);
	bool setUniform3iv(std::string name, int count, const int *v);
	bool setUniform4iv(std::string name, int count, const int *v);

	bool setUniformMat4(std::string name, Mat4f mat);
	bool setUniformMat3(std::string name, Mat4f mat);	//Takes the upper-left 3x3 portion of the matrix


	void setVShaderPath(std::string path);
	void setFShaderPath(std::string path);
};

#endif
