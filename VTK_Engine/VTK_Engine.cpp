// VTK_Engine.cpp : Defines the entry point for the application.
//

#include "VTK_Engine.h"
#include "vtkActor.h"
#include "vtkNew.h"

using namespace std;

int main()
{
	cout << "Hello CMake." << endl;
	cout << "Hello VTK." << endl;

	vtkNew<vtkActor> actor;
	actor->Print(cout);

	return 0;
}
