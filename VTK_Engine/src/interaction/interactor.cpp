#include "interaction/interactor.h"

Interactor::Interactor(vtkRenderWindow* renderWindow) {
	this->SetRenderWindow(renderWindow);
	vtkNew<vtkInteractorStyleTrackballCamera> style;
	this->SetInteractorStyle(style);
}

Interactor::~Interactor() {
}