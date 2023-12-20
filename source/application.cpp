#include <astra/application.h>

namespace Astra {

	Application* Application::s_Instance;
	WideChar Application::s_WindowClass[] = TEXT("AstraWindow");

	Application::Application()
	{
		s_Instance = this;
		m_Running = true;
		m_InstanceHandle = GetModuleHandle(NULL);

		WNDCLASS wc = { 0 };
		wc.style = CS_DBLCLKS;
		wc.lpfnWndProc = Application::ProcessMessage;
		wc.hInstance = m_InstanceHandle;
		wc.hbrBackground = CreateSolidBrush(RGB(10, 11, 13));
		wc.lpszClassName = s_WindowClass;

		if (!RegisterClass(&wc)) 
		{
			MessageBox(NULL, TEXT("Window Class Registration Failed!"), TEXT("Error"), MB_OK | MB_ICONERROR);
			Close();
		}

		m_Window = MakeUnique<Window>(m_InstanceHandle, s_WindowClass);

		if (m_Window->GetHandle() == NULL)
		{
			Close();
		}
	}

	Application::~Application()
	{
		UnregisterClass(s_WindowClass, m_InstanceHandle);
	}

	void Application::Run()
	{
		while (m_Running)
		{
			MSG msg;
			while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	void Application::Close()
	{
		m_Running = false;
	}

	LRESULT CALLBACK Application::ProcessMessage(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_DESTROY:
			{
				auto& app = Application::Get();
				
				if (app.m_Window->GetHandle() == handle)
				{
					app.Close();
				}
			}
			break;
		}

		return DefWindowProc(handle, message, wParam, lParam);
	}

	Application& Application::Get()
	{
		return *s_Instance;
	}
}