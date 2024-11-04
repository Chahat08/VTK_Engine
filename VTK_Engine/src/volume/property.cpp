#include "volume/property.h"

VolumeProperty::VolumeProperty() {
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