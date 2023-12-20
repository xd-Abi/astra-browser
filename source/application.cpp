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
		wc.lpfnWndProc = DefWindowProc;
		wc.hInstance = m_InstanceHandle;
		wc.hbrBackground = CreateSolidBrush(RGB(10, 11, 13));
		wc.lpszClassName = s_WindowClass;

		if (RegisterClass(&wc)) 
		{
			MessageBox(NULL, TEXT("Window Class Registration Failed!"), TEXT("Error"), MB_OK | MB_ICONERROR);
			Close();
		}
	}

	Application::~Application()
	{}

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

	Application& Application::Get()
	{
		return *s_Instance;
	}
}