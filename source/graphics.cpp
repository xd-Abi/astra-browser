#include <astra/graphics.h>

namespace Astra {

	GraphicsContext::GraphicsContext(HWND handle)
	{
		PIXELFORMATDESCRIPTOR pfd = { 0 };
		pfd.nSize = sizeof(pfd);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;

		HDC hdc = GetDC(handle);
		int pixelFormat = ChoosePixelFormat(hdc, &pfd);
		SetPixelFormat(hdc, pixelFormat, &pfd);
		ReleaseDC(handle, hdc);

		m_Hdc = GetDC(handle);
		m_OpenGLContext = wglCreateContext(hdc);
		wglMakeCurrent(hdc, m_OpenGLContext);
	}

	GraphicsContext::~GraphicsContext()
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(m_OpenGLContext);
	}

	void GraphicsContext::SwapBuffers()
	{
		::SwapBuffers(m_Hdc);
	}
}