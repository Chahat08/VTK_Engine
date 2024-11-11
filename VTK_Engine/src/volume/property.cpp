#include "volume/property.h"
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkNamedColors.h>

VolumeProperty::VolumeProperty() {
	this->ShadeOn();
	this->SetInterpolationTypeToLinear();
	m_colorTransferFunction = vtkColorTransferFunction::New();
	m_scalarOpacity = vtkPiecewiseFunction::New();
	this->SetColor(m_colorTransferFunction);
	this->SetScalarOpacity(m_scalarOpacity);
	setColorPoints(FrontendData::colorStops);
	setOpacityPoints(FrontendData::opacityPoints);
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

void VolumeProperty::setColorPoints(std::vector<FrontendData::ColorGradientStopPoint>& colorPoints) {
	m_colorTransferFunction->RemoveAllPoints();
	for (auto& colorPoint : colorPoints) {
		std::vector<float> colorData = FrontendData::getColor(colorPoint.color);
		std::cout << "Color data: " << colorPoint.position << ",r: "<< colorData[0] << " ,g: " << colorData[1] << " ,b: " << colorData[2] << std::endl;
		m_colorTransferFunction->AddRGBPoint(colorPoint.position, 
			colorData[0],
			colorData[1],
			colorData[2]);
	}
	this->Modified();
}

void VolumeProperty::setOpacityPoints(std::vector<FrontendData::OpacityControlPoint>& opacityPoints) {
	m_scalarOpacity->RemoveAllPoints();
	for (auto& opacityPoint : opacityPoints) {
		std::cout << "opacity: intensity: " << opacityPoint.value << "opacity: " << opacityPoint.opacity << std::endl;
		m_scalarOpacity->AddPoint(opacityPoint.value, opacityPoint.opacity);
	}
	this->Modified();
}