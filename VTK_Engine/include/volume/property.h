#ifndef PROPERTY_H
#define PROPERTY_H

#include <vtkVolumeProperty.h>

class VolumeProperty :public vtkVolumeProperty {
public:
	VolumeProperty();
	~VolumeProperty();

	void setInterpolationType(std::string type);
};

#endif