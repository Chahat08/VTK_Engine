#include "interaction/frontendData.h"

using namespace FrontendData;

bool OpacityControlPoint::operator==(const OpacityControlPoint& other) const{
	return this->value == other.value && this->opacity == other.opacity;
}

bool ColorGradientStopPoint::operator==(const ColorGradientStopPoint& other) const {
	return this->position == other.position && this->color == other.color;
}

std::vector<int> FrontendData::getColor(const std::string& color){
	int r = std::stoi(color.substr(1, 2), nullptr, 16) / 255;
	int g = std::stoi(color.substr(3, 2), nullptr, 16) / 255;
	int b = std::stoi(color.substr(5, 2), nullptr, 16) / 255;
	
    return { r, g, b };
}