#include <astra/application.h>

#include <gl/GL.h>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_win32.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

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

		HRESULT hr = CreateCoreWebView2EnvironmentWithOptions(NULL, NULL, NULL,
			Microsoft::WRL::Callback< ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
				[this](HRESULT result, ICoreWebView2Environment* env) -> HRESULT
				{
					RETURN_IF_FAILED(result);
					m_WebViewEnvironment = env;

					return S_OK;
				}
			).Get());

		m_WebViewEnvironment->CreateCoreWebView2Controller(m_Window->GetHandle(),
			Microsoft::WRL::Callback< ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
				[this](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT {

					if (controller != nullptr)
					{
						// @TODO Handle Error
						m_WebViewController = controller;
						m_WebViewController->get_CoreWebView2(&m_WebView);
					}

					wil::com_ptr<ICoreWebView2Settings> settings;
					m_WebView->get_Settings(&settings);
					settings->put_IsScriptEnabled(TRUE);
					settings->put_AreDefaultScriptDialogsEnabled(TRUE);
					settings->put_IsWebMessageEnabled(TRUE);

					RECT clientRect;
					GetClientRect(m_Window->GetHandle(), &clientRect);

					clientRect.top = 50;
					m_WebViewController->put_Bounds(clientRect);
					m_WebView->Navigate(TEXT("https://google.com"));

					return S_OK;
				}
			).Get());

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		ImGui::StyleColorsDark();

		ImGui_ImplWin32_InitForOpenGL(m_Window->GetHandle());
		ImGui_ImplOpenGL3_Init();
	}

	Application::~Application()
	{
		UnregisterClass(s_WindowClass, m_InstanceHandle);
	}

	void Application::Run()
	{
		bool show_demo_window = true;

		while (m_Running)
		{
			MSG msg;
			while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
			
			{
				ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove;
				flags |= ImGuiWindowFlags_NoResize;
				flags |= ImGuiWindowFlags_NoCollapse;
				flags |= ImGuiWindowFlags_NoTitleBar;
				flags |= ImGuiWindowFlags_NoScrollbar;

				ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.0392f, 0.043f, 0.050f, 1.00f });
				ImGui::PushStyleColor(ImGuiCol_Border, { 0,0,0,0 });
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 15, 0 });
				ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
				ImGui::SetNextWindowSize({ (float)m_Window->GetWidth(), 50 });
				ImGui::Begin("Application Caption", nullptr, flags);

				static float trafficLightRadius = 7;
				static float trafficLightSpacing = 7;
				ImVec2 contentRegion = ImGui::GetContentRegionAvail();
				ImGui::GetWindowDrawList()->AddCircleFilled({ contentRegion.x, 25 }, trafficLightRadius, IM_COL32(244, 90, 85, 255));
				ImGui::GetWindowDrawList()->AddCircleFilled({ contentRegion.x - trafficLightRadius * 2 - trafficLightSpacing, 25 }, trafficLightRadius, IM_COL32(246, 179, 46, 255));
				ImGui::GetWindowDrawList()->AddCircleFilled({ contentRegion.x - trafficLightRadius * 4 - trafficLightSpacing * 2, 25 }, trafficLightRadius, IM_COL32(38, 186, 62, 255));
	
				ImGui::End();
				ImGui::PopStyleVar(1);
				ImGui::PopStyleColor(2);
			}

			ImGui::Render();
			glViewport(0, 0, m_Window->GetWidth(), m_Window->GetHeight());
			glClearColor(0.0392f, 0.043f, 0.050f, 1.00f);
			glClear(GL_COLOR_BUFFER_BIT);

			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			m_Window->OnUpdate();
		}
	}

	void Application::Close()
	{
		m_Running = false;
	}

	LRESULT CALLBACK Application::ProcessMessage(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
	{
		static const RECT borderThickness = { 4, 4, 4, 4 };
		static const int captionThickness = 50;
		ImGui_ImplWin32_WndProcHandler(handle, message, wParam, lParam);

		switch (message)
		{
		case WM_NCCALCSIZE:
		{
			const int resizeBorderX = GetSystemMetrics(SM_CXFRAME);
			const int resizeBorderY = GetSystemMetrics(SM_CYFRAME);

			NCCALCSIZE_PARAMS* params = (NCCALCSIZE_PARAMS*)lParam;
			RECT* requestedClientRect = params->rgrc;

			requestedClientRect->right -= resizeBorderX;
			requestedClientRect->left += resizeBorderX;
			requestedClientRect->bottom -= resizeBorderY;

			/**
			 * NOTE: Top borders are to be handled differently.
			 *
			 * Contracting by 1 on Windows 11 seems to give a small area
			 * for resizing whilst not showing a white border.
			 */
			requestedClientRect->top += 0;
			return 0;
		}
		break;
		case WM_NCHITTEST:
		{
			POINT point = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
			ScreenToClient(handle, &point);

			if (!IsMaximized(handle))
			{
				RECT clientRect;
				GetClientRect(handle, &clientRect);

				const int verticalBorderSize = GetSystemMetrics(SM_CYFRAME);
				int hit = 0;

				enum {
					left = 1,
					top = 2,
					right = 4,
					bottom = 8,
				};

				if (point.x <= borderThickness.left)
				{
					hit |= left;
				}
				if (point.x >= clientRect.right - borderThickness.right)
				{
					hit |= right;
				}
				if (point.y <= borderThickness.top || point.y < verticalBorderSize)
				{
					hit |= top;
				}
				if (point.y >= clientRect.bottom - borderThickness.bottom)
				{
					hit |= bottom;
				}

				if (hit & top && hit & left)        return HTTOPLEFT;
				if (hit & top && hit & right)       return HTTOPRIGHT;
				if (hit & bottom && hit & left)     return HTBOTTOMLEFT;
				if (hit & bottom && hit & right)    return HTBOTTOMRIGHT;
				if (hit & left)                     return HTLEFT;
				if (hit & top)                      return HTTOP;
				if (hit & right)                    return HTRIGHT;
				if (hit & bottom)                   return HTBOTTOM;
			}

			if (point.y <= (LONG)captionThickness)
			{
				return HTCAPTION;
			}

			return HTCLIENT;
		}
		break;
		case WM_SIZE:
		{
			uint32 width = LOWORD(lParam);
			uint32 height = HIWORD(lParam);

			auto& app = Application::Get();

			if (app.m_Window != nullptr)
			{
				app.m_Window->m_Width = width;
				app.m_Window->m_Height = height;
			
				RECT clientRect;
				GetClientRect(handle, &clientRect);

				clientRect.top = captionThickness;
				app.m_WebViewController->put_Bounds(clientRect);
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