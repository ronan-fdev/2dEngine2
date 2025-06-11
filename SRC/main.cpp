#include <iostream>
#include "Application.h"

//These lines helps in bring the descreate GPU. But it  has some problems, due to which it has been halted.
//extern "C" {
//	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
//}

int main()
{
	auto& app = Application::GetInstance();
	app.Run();
	return 0;
}
