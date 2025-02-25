#include "volume/property.h"
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkNamedColors.h>
#include <vtkContourValues.h>

VolumeProperty::VolumeProperty() {
	this->SetShade(FrontendData::defaultShading);
	setInterpolationType(FrontendData::defaultInterpolationType);

	m_colorTransferFunction = vtkColorTransferFunction::New();
	m_scalarOpacity = vtkPiecewiseFunction::New();
	this->SetColor(m_colorTransferFunction);
	this->SetScalarOpacity(m_scalarOpacity);
	setColorPoints(FrontendData::defaultColorStops);
	setOpacityPoints(FrontendData::defaultOpacityPoints);

	// TODO: Non independent componenets handling, like RGB/RGBA datasets.
	//this->SetIndependentComponents(false);
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
	this->m_colorTransferFunction->Modified();
	this->Modified();
}

void VolumeProperty::setOpacityPoints(std::vector<FrontendData::OpacityControlPoint>& opacityPoints) {
	m_scalarOpacity->RemoveAllPoints();
	for (auto& opacityPoint : opacityPoints) {
		std::cout << "opacity: intensity: " << opacityPoint.value << "opacity: " << opacityPoint.opacity << std::endl;
		m_scalarOpacity->AddPoint(opacityPoint.value, opacityPoint.opacity);
	}
	this->m_scalarOpacity->Modified();
	this->Modified();
}

void VolumeProperty::setIsovalues(std::vector<float>& isovalues) {
	for (int i = 0; i < isovalues.size(); ++i) {
		std::cout << "Isovalues: " << isovalues[i] << std::endl;
		this->GetIsoSurfaceValues()->SetValue(i, isovalues[i]);
	}
	this->IsoSurfaceValues->Modified();
	this->Modified();
}