#include <astra/application.h>

namespace Astra {

	Application* Application::s_Instance;

	Application::Application()
	{
		m_Running = true;
	}

	Application::~Application()
	{}

	void Application::Run()
	{
		while (m_Running)
		{

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