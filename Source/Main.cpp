#include "SDL2/SDL.h"
#include "Window.h"
#include "Nova.h"

int main(int argc, char* argv[])
{

	Nova::Window* window = new Nova::Window("opengl window", 720, 720);

	//window->SetVSync(false);

	if (window->Init())
	{
		Nova::Nova application(window);
		application.Start();
	}
	else
	{
		delete window;
	}

	return 0;
}

