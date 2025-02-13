#ifndef MAPPER_H
#define MAPPER_H

#include <string>

#include <vtkOpenGLGPUVolumeRayCastMapper.h>
#include "reader/reader.h"
#include "interaction/frontendData.h"
#include "volume/property.h"


class VolumeMapper : public vtkOpenGLGPUVolumeRayCastMapper {
public:
	VolumeMapper();
	~VolumeMapper();

	void setBlendMode(std::string blendMode = "Composite");
	std::string getBlendMode();

private:
	std::string blendMode;
};

#endif
