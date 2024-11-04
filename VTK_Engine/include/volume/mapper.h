#ifndef MAPPER_H
#define MAPPER_H

#include <vtkOpenGLGPUVolumeRayCastMapper.h>
#include "reader/reader.h"

class VolumeMapper : public vtkOpenGLGPUVolumeRayCastMapper {
public:
	VolumeMapper();
	~VolumeMapper();


	void setBlendMode(std::string blendMode = "IsoSurface");
};

#endif
