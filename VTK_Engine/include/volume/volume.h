#ifndef VOLUME_H
#define VOLUME_H

#include <vtkVolume.h>
#include "reader/reader.h"

class Volume :public vtkVolume {
public:
	Volume();
	~Volume();

	std::vector<std::pair<double, double>> getVolumeBounds();
	void setVolumeParameters(VolumeReader* reader);
	void setSlicePlane(double planeAngle);
	void readVoxels(VolumeReader* reader);
	void printSelf() const;

	double intensityRange[2];
	int* dimensions;
	double* spacing;
	double* origin;
	int* extent;

private:
};

#endif