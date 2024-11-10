#include "interaction/frontendData.h"

using namespace FrontendData;

bool OpacityControlPoint::operator==(const OpacityControlPoint& other) const{
	return this->value == other.value && this->opacity == other.opacity;
}

bool ColorGradientStopPoint::operator==(const ColorGradientStopPoint& other) const {
	return this->position == other.position && this->color == other.color;
}

std::vector<float> FrontendData::getColor(const std::string& color){
	std::vector<float> colorVec;
	colorVec.push_back(colors->GetColor3d(color).GetData()[0]);
	colorVec.push_back(colors->GetColor3d(color).GetData()[1]);
	colorVec.push_back(colors->GetColor3d(color).GetData()[2]);
	return colorVec;
}