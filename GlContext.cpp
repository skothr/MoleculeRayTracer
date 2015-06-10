#include "GlContext.h"

#include <iostream>

#include "Shaders.h"


/////GL CONTEXT/////
//ShaderProgram *GlContext::textSP = nullptr;
//GLuint GlContext::text_vbID = 0;

int GlContext::activeIndex = -1;
std::vector<GlContext*> GlContext::contexts;

GlContext::GlContext()
{
	//contexts.push_back(this);
	//thisIndex = contexts.size() - 1;
}

GlContext::~GlContext()
{
	//if(activeIndex == thisIndex)
	//{
	//	activeIndex = -1;
		wglMakeCurrent(nullptr, nullptr);
	//}

	//for(unsigned int i = thisIndex + 1; i < contexts.size(); i++)
	//{
	//	if(contexts[i]->isActive())
	//		activeIndex--;
	//	contexts[i]->thisIndex--;
	//}

	//contexts.erase(contexts.begin() + thisIndex);

	wglDeleteContext(m_hGLRC);
	ReleaseDC(m_hWnd, m_hDC);
}

void GlContext::setUpPixelFormat(HDC dc)
{
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		16,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		16,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	int pixelFormat = ChoosePixelFormat(dc, &pfd);
	if(!pixelFormat)
	{
		MessageBox(WindowFromDC(dc), L"ChoosePixelFormat has failed.", L"Error", MB_ICONERROR | MB_OK);
		exit(1);
	}

	if(SetPixelFormat(dc, pixelFormat, &pfd) != TRUE)
	{
		MessageBox(WindowFromDC(dc), L"SetPixelFormat has failed.", L"Error", MB_ICONERROR | MB_OK);
		exit(1);
	}

	//std::cout << "---" << pixelFormat << "\n";

}

void GlContext::setUpPalette()
{
	int pixelFormat = GetPixelFormat(m_hDC);
	PIXELFORMATDESCRIPTOR pfd;
	LOGPALETTE *pPal;
	int paletteSize;

	DescribePixelFormat(m_hDC, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	if(pfd.dwFlags & PFD_NEED_PALETTE)
		paletteSize = 1;
	else
		return;

	pPal = (LOGPALETTE*)malloc(sizeof(LOGPALETTE) + paletteSize*sizeof(PALETTEENTRY));

	pPal->palVersion = 0x300;
	pPal->palNumEntries = paletteSize;

	//RGB color palette
	int redMask = (1 << pfd.cRedBits) - 1;
	int greenMask = (1 << pfd.cGreenBits) - 1;
	int blueMask = (1 << pfd.cBlueBits) - 1;

	for(int i = 0; i < paletteSize; i++)
	{
		pPal->palPalEntry[i].peRed = (((i >> pfd.cRedShift) & redMask) * 25) / redMask;
		pPal->palPalEntry[i].peGreen = (((i >> pfd.cGreenShift) & greenMask) * 25) / greenMask;
		pPal->palPalEntry[i].peBlue = (((i >> pfd.cBlueShift) & blueMask) * 25) / blueMask;
		pPal->palPalEntry[i].peFlags = 0;
	}

	m_hPalette = CreatePalette(pPal);
	free(pPal);

	if(m_hPalette)
	{
		SelectPalette(m_hDC, m_hPalette, FALSE);
		RealizePalette(m_hDC);
	}
}

void GlContext::init(HWND hwnd)
{
	m_hWnd = hwnd;
	m_hDC = GetDC(m_hWnd);
	setUpPixelFormat(m_hDC);
	setUpPalette();

	/*
	GLint attribs[] = 
	{
	};
	*/
	
	m_hGLRC = wglCreateContext(m_hDC);

	setActive(m_hDC);
	
	GLint major, minor;

	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	std::cout << "OpenGL Version " << major << "." << minor << " context created.\n\n";
	
	GLenum err = glewInit();
	if(err != GLEW_OK)
		std::cout << glewGetErrorString(err) << "\n";

	//if(!textSP)
	//{
	//	loadResources();
	//}
	
	//TODO: Set up matrices

	//
	//glMatrixMode(GL_PROJECTION);
	//glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -5.0f, 5.0f);
}
/*
void GlContext::setActiveContext(GlContext *c)
{
	if(c)
		c->setActive();
	else
		wglMakeCurrent(nullptr, nullptr);
}
*/
void GlContext::setActive(HDC window)
{
	//if(activeIndex != thisIndex)
	//{
		HDC curr_dc = wglGetCurrentDC();

		if(curr_dc != window)
		{
			//wglMakeCurrent(nullptr, nullptr);

			if(!wglMakeCurrent(window, m_hGLRC))
			{
				std::cout << "Failed to make context active! --> " << GetLastError() << "\n";
			}
		}
	//	else
	//		activeIndex = thisIndex;
	//}
}

void GlContext::swapBuffers(HDC window)
{
	SwapBuffers(window);
}

bool GlContext::isActive(HDC window)
{
	HDC curr_dc = wglGetCurrentDC();

	return (curr_dc == window);//thisIndex == activeIndex;
}