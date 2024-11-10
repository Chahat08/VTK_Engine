#include "volume/property.h"
#include "interaction/frontendData.h"
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkNamedColors.h>

VolumeProperty::VolumeProperty() {
	m_colorTransferFunction = vtkColorTransferFunction::New();
	m_scalarOpacity = vtkPiecewiseFunction::New();
	this->SetColor(m_colorTransferFunction);
	this->SetScalarOpacity(m_scalarOpacity);
	setColorPoints();
	setOpacityPoints();
}

VolumeProperty::~VolumeProperty() {
	this->Delete();
}

void VolumeProperty::setInterpolationType(std::string type) {
	if (type == "Nearest") 
		this->SetInterpolationTypeToNearest();
	else if (type == "Linear") 
		this->SetInterpolationTypeToLinear();
	else 
		this->SetInterpolationTypeToLinear();
	
}

void VolumeProperty::setColorPoints() {
	m_colorTransferFunction->RemoveAllPoints();
	vtkNew<vtkNamedColors> colors;
	for (auto& colorPoint : FrontendData::colorStops) {
		std::vector<int> colorData = FrontendData::getColor(colorPoint.color);
		m_colorTransferFunction->AddRGBPoint(colorPoint.position, 
			colorData[0],
			colorData[1],
			colorData[2]);
	}
}

void VolumeProperty::setOpacityPoints() {
	m_scalarOpacity->RemoveAllPoints();
	for (auto& opacityPoint : FrontendData::opacityPoints) {
		m_scalarOpacity->AddPoint(opacityPoint.value, opacityPoint.opacity);
	}
}