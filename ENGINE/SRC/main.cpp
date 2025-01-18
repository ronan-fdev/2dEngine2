#include <iostream>

#include "../WINDOW/Window.h"
#include "../LOGGER/log.h"

int main()
{
	std::cout << "Hello World!" << std::endl;

	//Logger
	log::Init();

	Window::init(640, 480, "2DENGINE");

	while (!Window::shouldClose())
	{
		Window::clearScreen();
		Window::processInput();
		Window::processInput();
		if (Window::getdt() > 0.0f)
		{

		}
		Window::update();
	}
	Window::cleanup();
	return 0;
}
