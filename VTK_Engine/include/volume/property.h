#ifndef PROPERTY_H
#define PROPERTY_H

#include <vtkVolumeProperty.h>

class VolumeProperty :public vtkVolumeProperty {
public:
	VolumeProperty();
	~VolumeProperty();

	void setInterpolationType(std::string type);

	void setColorPoints();
	void setOpacityPoints();

private:
	vtkColorTransferFunction* m_colorTransferFunction;
	vtkPiecewiseFunction* m_scalarOpacity;
};

#endif