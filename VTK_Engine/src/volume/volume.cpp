#include "volume/volume.h"

Volume::Volume() {
}

Volume::~Volume() {
}

std::vector<std::pair<double, double>> Volume::getVolumeBounds() {
	double* bounds = this->GetBounds();
	std::vector<std::pair<double, double>> volumeBounds;
	for (int i = 0; i < 6; i += 2) {
		volumeBounds.push_back(std::make_pair(bounds[i], bounds[i + 1]));
	}
	return volumeBounds;
}