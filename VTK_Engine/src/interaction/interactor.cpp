#include "interaction/interactor.h"

Interactor::Interactor(vtkRenderWindow* renderWindow) {
	this->Initialize();
	this->SetRenderWindow(renderWindow);
	vtkNew<vtkInteractorStyleTrackballCamera> style;
	this->SetInteractorStyle(style);
}

Interactor::~Interactor() {
}