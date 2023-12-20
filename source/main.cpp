#include <astra/application.h>

int main(int, char**)
{
    auto app = new Astra::Application();
    app->Run();
    delete app;
}