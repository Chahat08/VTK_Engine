#ifndef PROPERTY_H
#define PROPERTY_H

#include <vtkVolumeProperty.h>
#include "interaction/frontendData.h"
#include "volume/mapper.h"

class VolumeProperty :public vtkVolumeProperty {
public:
	VolumeProperty();
	~VolumeProperty();

	void setInterpolationType(std::string type);

	void setColorPoints(std::vector<FrontendData::ColorGradientStopPoint>& colorPoints);
	void setOpacityPoints(std::vector<FrontendData::OpacityControlPoint>& opacityPoints);
	void setIsovalues(std::vector<float>& isovalues);

private:
	vtkColorTransferFunction* m_colorTransferFunction;
	vtkPiecewiseFunction* m_scalarOpacity;
};

#endif