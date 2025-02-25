#include "interaction/frontendData.h"

using namespace FrontendData;

bool OpacityControlPoint::operator==(const OpacityControlPoint& other) const{
	return this->value == other.value && this->opacity == other.opacity;
}

bool ColorGradientStopPoint::operator==(const ColorGradientStopPoint& other) const {
	return this->position == other.position && this->color == other.color;
}

bool BackgroundColor::operator==(const BackgroundColor& other) const {
	return this->color == other.color;
}

std::vector<float> FrontendData::getColor(const std::string& color){
	int r = std::stoi(color.substr(1, 2), nullptr, 16);
	int g = std::stoi(color.substr(3, 2), nullptr, 16);
	int b = std::stoi(color.substr(5, 2), nullptr, 16);
	
    return { r/255.0f, g/255.0f, b/255.0f };
}
