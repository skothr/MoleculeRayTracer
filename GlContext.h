#ifndef GL_CONTEXT_H
#define GL_CONTEXT_H

#include <Windows.h>
#include <GL/glew.h>
#include <vector>

class ShaderProgram;

class GlContext
{
private:
	//TODO: Go back to using multiple contexts (????)
	static std::vector<GlContext*> contexts;
	static int activeIndex;

	unsigned int thisIndex;

	HDC m_hDC = nullptr;
	HGLRC m_hGLRC = nullptr;
	HPALETTE m_hPalette = nullptr;
	HWND m_hWnd = nullptr;

	void setUpPalette();

public:

	GlContext();
	virtual ~GlContext();
	
	void setUpPixelFormat(HDC dc);

	//static void setActiveContext(GlContext *c);

	void init(HWND hwnd);
	void setActive(HDC window);
	void swapBuffers(HDC window);

	bool isActive(HDC window);
};


#endif	//GL_CONTEXT_H