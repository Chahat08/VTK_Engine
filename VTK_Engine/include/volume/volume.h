#ifndef VOLUME_H
#define VOLUME_H

#include <vtkVolume.h>

class Volume :public vtkVolume {
public:
	Volume();
	~Volume();

	std::vector<std::pair<double, double>> getVolumeBounds();
};

#endif