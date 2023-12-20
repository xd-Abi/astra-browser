#include <astra/application.h>

int main() 
{
	auto app = new Astra::Application();
	app->Run();
	delete app;
}