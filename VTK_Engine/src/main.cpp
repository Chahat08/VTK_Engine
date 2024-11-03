// VTK_Engine.cpp : Defines the entry point for the application.
//

#include "app/app.h"
#include <iostream>

int main()
{
	cout << "Hello CMake." << endl;
	cout << "Hello VTK." << endl;

	App::getInstance(
		800, 600, 100, 100, false, "VTK Engine"
	).run();

	return 0;
}
