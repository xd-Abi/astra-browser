#include <astra/window.h>

namespace Astra {

	Window::Window(HINSTANCE instanceHandle, const WideChar windowClass[])
	{
		m_Width = 1280;
		m_Height = 720;
		m_Handle = CreateWindow(
			windowClass,
			TEXT("Astra"),
			WS_OVERLAPPEDWINDOW,
			100,
			100,
			m_Width,
			m_Height,
			NULL,
			NULL,
			instanceHandle,
			NULL,
		);

		if (m_Handle == NULL)
		{
			MessageBox(NULL, TEXT("Window Creation Failed!"), TEXT("Error"), MB_OK | MB_ICONERROR);
			return;
		}

		ShowWindow(m_Handle, SW_SHOWNORMAL);
		UpdateWindow(m_Handle);
	}

	Window::~Window()
	{
		if (m_Handle != NULL)
		{
			DestroyWindow(m_Handle);
		}
	}

	HWND Window::GetHandle() const
	{
		return m_Handle;
	}

	uint32 Window::GetWidth() const
	{
		return m_Width;
	}

	uint32 Window::GetHeight() const
	{
		return m_Height;
	}
}