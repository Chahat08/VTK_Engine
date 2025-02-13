#include "volume/outline.h"
#include <vtkNew.h>
#include <vtkNamedColors.h>

VolumeOutline::VolumeOutline(VolumeReader* reader) {
	vtkNew<vtkNamedColors> colors;

	vtkNew<vtkOutlineFilter> outlineData;
	outlineData->SetInputConnection(reader->getOutputPort());
	outlineData->Update();

	vtkNew<vtkPolyDataMapper> mapOutline;
	mapOutline->SetInputConnection(outlineData->GetOutputPort());

	m_outlineActor = vtkActor::New();
	m_outlineActor->SetMapper(mapOutline);
	m_outlineActor->GetProperty()->SetColor(colors->GetColor3d("White").GetData());
}

vtkActor* VolumeOutline::getVolumeOutline() {
	return m_outlineActor;
}