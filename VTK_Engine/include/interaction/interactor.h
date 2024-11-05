#ifndef INTERACTOR_H
#define INTERACTOR_H

#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderWindow.h>

class Interactor :public vtkRenderWindowInteractor {
public:
	Interactor(vtkRenderWindow* renderWindow);
	~Interactor();
};

#endif