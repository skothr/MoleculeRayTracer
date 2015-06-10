#include <GL/glew.h>
//#include <SFML/System.hpp>
//#include <SFML/Window.hpp>

#include "Shaders.h"

#include <iostream>
#include <fstream>


//Shader definitions

Shader::Shader()
{
	shaderType = GL_INVALID_ENUM;
	pathName.erase();
	fileName = "[No specified file]";
	srcCode.erase();

	shaderID = NULL;
}

Shader::~Shader()
{
	shaderType = GL_INVALID_ENUM;
	srcCode.erase();

	if(shaderID != NULL)
		glDeleteShader(shaderID);

	shaderID = NULL;
}

void Shader::vBareShader(GLuint programID)
{
	shaderType = GL_VERTEX_SHADER;

	fileName = "[None]";
	
	if(shaderID !=NULL)
		glDeleteShader(shaderID);

	//Create shader
	shaderID = glCreateShader(shaderType);

	//Attach to programID
	glAttachShader(programID, shaderID);
	
	//Basic vertex shader source
	srcCode.erase();
	srcCode += "void main()\n";
	srcCode += "{\n";
	srcCode += "	gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex;\n";
	srcCode += "	gl_FrontColor = gl_Color;\n";
	srcCode += "}\n";

	src = (GLchar*)srcCode.c_str();
	
	//Load source code
	glShaderSource(shaderID, 1, (const GLchar**)&src, NULL);

	compile();

	return;
}

void Shader::fBareShader(GLuint programID)
{
	shaderType = GL_FRAGMENT_SHADER;

	fileName = "[None]";

	if(shaderID !=NULL)
		glDeleteShader(shaderID);

	//Create shader
	shaderID = glCreateShader(shaderType);

	//Attach to programID
	glAttachShader(programID, shaderID);

	//Basic fragment shader source
	srcCode.erase();
	srcCode += "void main()\n";
	srcCode += "{\n";
	srcCode += "	gl_FragColor = gl_Color;\n";
	srcCode += "}\n";

	src = (GLchar*)srcCode.c_str();
	
	//Load source code
	glShaderSource(shaderID, 1, (const GLchar**)&src, NULL);

	compile();

	return;
}

void Shader::setPath(const char *pPath)
{
	//size_t found;

#if defined(__APPLE__)
    pathName.erase();
    pathName = pPath;
    found = pathName.find_last_of("/");

    if(found != string::npos)
    {
        pathName = pathName.substr(0,found);
        pathName += "/";
    }
#endif
#if defined(_WIN32)

    //pathName.erase();
    //pathName = pPath;

    //found = pathName.find_last_of("/\\");

    //if(found != std::string::npos)
    //{
    //    pathName = pathName.substr(0,found);
    //    pathName += "\\";
    //}
    //std::cout << "\n\n Trimmed path:" << pathName << "\n\n";

#endif

	return;
}

bool Shader::loadFromFile(GLuint programID, std::string name, GLenum type)
{
	fileName = name;

	return load(programID);
}

bool Shader::compile()
{
	const int LOG_MAXLENGTH = 8096;
	GLchar logBuffer[LOG_MAXLENGTH];
	GLsizei logLength;
	//GLint ans;

	GLint compileSucceeded;
	
	//Compile shader
	glCompileShader(shaderID);
	
	//Check if compile failed
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileSucceeded);

	std::string type;

	switch(shaderType)
	{
	case GL_VERTEX_SHADER:
		type = "VERTEX";
		break;
	case GL_FRAGMENT_SHADER:
		type = "FRAGMENT";
		break;
	case GL_GEOMETRY_SHADER:
		type = "GEOMETRY";
		break;
	default:
		type = "UNKNOWN_TYPE";
		break;
	}

	if(!compileSucceeded)
	{
		std::cout << type << " shader " << fileName << " did not compile!\n";

		glGetShaderInfoLog(shaderID, LOG_MAXLENGTH, &logLength, logBuffer);

		std::cout << "\n Error Log:\n";
		std::cout << "-----------------------------------------------\n";
		std::cout << logBuffer << "\n";
		std::cout << "----------(End of Error Log)-----------------\n";

		return false;
    }

	return true;
}

bool Shader::load(GLuint programID)
{
	//Read file
	readShaderFile();

	//Check for invalid shader type
	if(shaderType == GL_INVALID_ENUM)
	{
		std::cout << "ERROR: Invalid shader type. Could not load.\n";
		return false;
	}

	//Delete old shader if applicable
	if(shaderID != NULL)
	{
		glDetachShader(programID, shaderID);
		glDeleteShader(shaderID);
		shaderID = NULL;
	}

	//Create shader
	shaderID = glCreateShader(shaderType);

	//Attach to programID
	glAttachShader(programID, shaderID);

	//Load source code
	glShaderSource(shaderID, 1, (const GLchar**)&src, NULL);

	if(!compile())
		return false;

	return true;
}

bool Shader::readShaderFile()
{
	std::fstream shaderFile;
	std::string lineBuff;
	std::string fullPath;

	fullPath = pathName + fileName;

	//Open file
	shaderFile.open (fullPath.c_str(), std::ifstream::in);

	if(!shaderFile)
	{
		std::cout << "ERROR: Could not open shader file:  " << fileName << "\n";
		return false;
	}
	
	//Put file contents into srcCode
	srcCode.erase();
	while (!shaderFile.eof())
	{
		getline(shaderFile, lineBuff);
		lineBuff += '\n';
		srcCode += lineBuff;
	}

	//Close file
	shaderFile.close();

	src = (GLchar*)srcCode.c_str();

	return true;
}

GLuint Shader::getShaderID()
{
	return shaderID;
}

void Shader::print()
{
	int size = srcCode.size();
	for(int i = 0; i < size; i++)
	{
		std::cout << srcCode[i];
	}

	return;
}

//ShaderProgram definitions

ShaderProgram::ShaderProgram()
{
	programID = glCreateProgram();

	vShader.vBareShader(programID);
	fShader.fBareShader(programID);
}

ShaderProgram::ShaderProgram(const char* argv_0)
{
	programID = glCreateProgram();

	setVShaderPath(argv_0);
	vShader.vBareShader(programID);
	
	setFShaderPath(argv_0);
	fShader.fBareShader(programID);
}

ShaderProgram::~ShaderProgram() { }


bool ShaderProgram::canRunShaders()
{
	return glewIsSupported("GL_VERSION_2_0") != 0;
}

bool ShaderProgram::loadShaders(std::string vShaderFile, std::string fShaderFile)
{
	bool succeeded = true;

	//Load vertex shader
	if(!vShader.loadFromFile(programID, vShaderFile, GL_VERTEX_SHADER))
		succeeded = false;

	//Load fragment shader
	if(!fShader.loadFromFile(programID, fShaderFile, GL_FRAGMENT_SHADER))
		succeeded = false;

	//Link program
	if(!linkProgram())
		succeeded = false;

	return succeeded;
}

//Compile the shader program (basically copied from starter code)
bool ShaderProgram::linkProgram()
{
	const int LOG_MAXLENGTH = 8096;
	GLchar logBuffer[LOG_MAXLENGTH];
	GLsizei logLength;
	GLint ans;

	GLint linkSucceeded;
	
	glLinkProgram(programID);
	
	GLint isLinked = 0;
	glGetProgramiv(programID, GL_LINK_STATUS, &isLinked);
	if(!isLinked)
	{
		std::cout << "ERROR: Shader program linking failed.\n";
	}

	glGetProgramiv(programID, GL_LINK_STATUS, &linkSucceeded);
	if(!linkSucceeded)
	{
		glGetProgramInfoLog(programID, LOG_MAXLENGTH, &logLength, logBuffer);
		std::cout << "\n Program Error Info Log: ("<< logLength <<" chars)\n";
		std::cout <<"-------------------------------------------------------\n";
		std::cout << logBuffer << "\n";
		std::cout <<"----------(end Program Error Info Log)-----------------\n";

		return false;
    }

	std::cout << "Shader Program linking info: " "\n";
	std::cout << "------------------------------" "\n";
	glGetProgramiv(programID, GL_ATTACHED_SHADERS,  &ans);
	std::cout << "\t" << ans << " attached shaders found,\n";
	glGetProgramiv(programID, GL_ACTIVE_ATTRIBUTES, &ans);
	std::cout << "\t" << ans << " active attributes found,\n";
	glGetProgramiv(programID, GL_ACTIVE_UNIFORMS, &ans);
	std::cout << "\t" << ans << " active uniforms found," "\n\n";
	glGetProgramInfoLog(programID, LOG_MAXLENGTH, &logLength, logBuffer);
	std::cout << "\nLinking Log:\n";
	std::cout <<"---------------------------------------------------------\n";
	std::cout << logBuffer << "\n";
	std::cout <<"----------(End of Linking Log)-----------------\n";

	getUniformLocations();
	
	return true;
}

void ShaderProgram::setActive()
{
	glUseProgram(programID);

	return;
}

void ShaderProgram::getUniformLocations()
{
	int numUniforms;
	glGetProgramiv(programID, GL_ACTIVE_UNIFORMS, &numUniforms);

	for(int i = 0; i < numUniforms; i++)
	{
		int nameLength, num;
		GLenum type = GL_ZERO;
		const int MAX_LENGTH = 128;
		char name[MAX_LENGTH];

		//Get uniform info
		glGetActiveUniform(programID, GLuint(i), MAX_LENGTH, &nameLength, &num, &type, name);
		
		//Store uniform info
		uniformLocations.push_back(glGetUniformLocation(programID, name));
		uniformNames.push_back(name);
	}

	return;
}

int ShaderProgram::getUniformIndex(std::string name)
{
	int size = uniformNames.size();

	for(int i = 0; i < size; i++)
	{
		if(name == uniformNames[i])
		{
			return i;
		}
	}

	return -1;
}

bool ShaderProgram::setUniform1f(std::string name, float v0)
{
	int index = getUniformIndex(name);

	if(index < 0) return false;

	setActive();
	glUniform1f(uniformLocations[index], v0);

	return true;
}

bool ShaderProgram::setUniform2f(std::string name, float v0, float v1)
{
	int index = getUniformIndex(name);
	
	if(index < 0) return false;

	setActive();
	glUniform2f(uniformLocations[index], v0, v1);

	return true;
}

bool ShaderProgram::setUniform3f(std::string name, float v0, float v1, float v2)
{
	int index = getUniformIndex(name);
	
	if(index < 0) return false;

	setActive();
	glUniform3f(uniformLocations[index], v0, v1, v2);

	return true;
}

bool ShaderProgram::setUniform4f(std::string name, float v0, float v1, float v2, float v3)
{
	int index = getUniformIndex(name);
	
	if(index < 0) return false;

	setActive();
	glUniform4f(uniformLocations[index], v0, v1, v2, v3);

	return true;
}


bool ShaderProgram::setUniform1i(std::string name, int v0)
{
	int index = getUniformIndex(name);
	
	if(index < 0) return false;

	setActive();
	glUniform1i(uniformLocations[index], v0);

	return true;
}

bool ShaderProgram::setUniform2i(std::string name, int v0, int v1)
{
	int index = getUniformIndex(name);
	
	if(index < 0) return false;

	setActive();
	glUniform2i(uniformLocations[index], v0, v1);

	return true;
}

bool ShaderProgram::setUniform3i(std::string name, int v0, int v1, int v2)
{
	int index = getUniformIndex(name);
	
	if(index < 0) return false;

	setActive();
	glUniform3i(uniformLocations[index], v0, v1, v2);

	return true;
}

bool ShaderProgram::setUniform4i(std::string name, int v0, int v1, int v2, int v3)
{
	int index = getUniformIndex(name);
	
	if(index < 0) return false;

	setActive();
	glUniform4i(uniformLocations[index], v0, v1, v2, v3);

	return true;
}


bool ShaderProgram::setUniform1fv(std::string name, int count, const float *v)
{
	int index = getUniformIndex(name);

	if(index < 0) return false;

	setActive();
	glUniform1fv(uniformLocations[index], count, v);

	return true;
}

bool ShaderProgram::setUniform2fv(std::string name, int count, const float *v)
{
	int index = getUniformIndex(name);
	
	if(index < 0) return false;

	setActive();
	glUniform2fv(uniformLocations[index], count, v);

	return true;
}

bool ShaderProgram::setUniform3fv(std::string name, int count, const float *v)
{
	int index = getUniformIndex(name);
	
	if(index < 0) return false;

	setActive();
	glUniform3fv(uniformLocations[index], count, v);

	return true;
}

bool ShaderProgram::setUniform4fv(std::string name, int count, const float *v)
{
	int index = getUniformIndex(name);
	
	if(index < 0) return false;

	setActive();
	glUniform4fv(uniformLocations[index], count, v);

	return true;
}


bool ShaderProgram::setUniform1iv(std::string name, int count, const int *v)
{
	int index = getUniformIndex(name);
	
	if(index < 0) return false;

	setActive();
	glUniform1iv(uniformLocations[index], count, v);

	return true;
}

bool ShaderProgram::setUniform2iv(std::string name, int count, const int *v)
{
	int index = getUniformIndex(name);
	
	if(index < 0) return false;

	setActive();
	glUniform2iv(uniformLocations[index], count, v);

	return true;
}

bool ShaderProgram::setUniform3iv(std::string name, int count, const int *v)
{
	int index = getUniformIndex(name);
	
	if(index < 0) return false;

	setActive();
	glUniform3iv(uniformLocations[index], count, v);

	return true;
}

bool ShaderProgram::setUniform4iv(std::string name, int count, const int *v)
{
	int index = getUniformIndex(name);
	
	if(index < 0) return false;

	setActive();
	glUniform4iv(uniformLocations[index], count, v);

	return true;
}


bool ShaderProgram::setUniformMat4(std::string name, Mat4f mat)
{
	int index = getUniformIndex(name);
	
	if(index < 0) return false;

	setActive();

	float *arr = mat.toFArray();
	glUniformMatrix4fv(uniformLocations[index], 1, false, arr);
	delete[] arr;

	return true;
}

bool ShaderProgram::setUniformMat3(std::string name, Mat4f mat)
{
	return false;
}



GLuint ShaderProgram::getProgramID()
{
	return programID;
}

GLuint ShaderProgram::getVShaderID()
{
	return vShader.getShaderID();
}

GLuint ShaderProgram::getFShaderID()
{
	return fShader.getShaderID();
}
	

void ShaderProgram::setVShaderPath(std::string path)
{
	vShader.setPath(path.c_str());

	return;
}

void ShaderProgram::setFShaderPath(std::string path)
{
	fShader.setPath(path.c_str());

	return;
}