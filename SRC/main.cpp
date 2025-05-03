#include <iostream>
#include "Application.h"

int main()
{
	auto& app = Application::GetInstance();
	app.Run();
	return 0;
}
