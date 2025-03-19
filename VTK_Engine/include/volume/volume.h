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
	void setSlicePlane(double planeAngle , double* normal, double* axis);
	void moveSliceOriginInDirection(double offset, double* direction);
	void readVoxels(VolumeReader* reader);
	void printSelf() const;

	double intensityRange[2];
	int* dimensions;
	double* spacing;
	double* origin;
	int* extent;

	std::vector<double> sliceOrigin;

private:
};

#endif