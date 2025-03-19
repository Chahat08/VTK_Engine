#include "volume/volume.h"
#include <vtkPlane.h>
#include <vtkTransform.h>
#include <vtkMath.h>
#include "volume/property.h"

Volume::Volume() {
}

Volume::~Volume() {
	this->Delete();
}

std::vector<std::pair<double, double>> Volume::getVolumeBounds() {
	double* bounds = this->GetBounds();
	std::vector<std::pair<double, double>> volumeBounds;
	for (int i = 0; i < 6; i += 2) {
		volumeBounds.push_back(std::make_pair(bounds[i], bounds[i + 1]));
	}
	return volumeBounds;
}

void Volume::setVolumeParameters(VolumeReader* reader) {
	reader->update();
	vtkImageData* imageData = reader->getImageData();
	imageData->GetScalarRange(intensityRange);
	dimensions = imageData->GetDimensions();
	spacing = imageData->GetSpacing();
	origin = imageData->GetOrigin();
	extent = imageData->GetExtent();

	std::vector<std::pair<double, double>> bounds = getVolumeBounds();
	sliceOrigin = {
		(bounds[0].first + bounds[0].second) / 2.0,
		(bounds[1].first + bounds[1].second) / 2.0,
		(bounds[2].first + bounds[2].second) / 2.0
	};
}

void Volume::printSelf() const{
	std::cout << "Volume Info: " << std::endl;
	std::cout << "INTENSITY RANGE: " << intensityRange[0] << " " << intensityRange[1] << std::endl;
	std::cout << "DIMENSIONS: " << dimensions[0] << " " << dimensions[1] <<" "<<dimensions[2] << std::endl;
	std::cout << "SPACING: " << spacing[0] << " " << spacing[1] <<" "<<spacing[2] << std::endl;
	std::cout << "ORIGIN: " << origin[0] << " " << origin[1] <<" "<<origin[2] << std::endl;
	std::cout << "EXTENT: " << extent[0] << " " << extent[1] << ", " << extent[2] << " " << extent[3] << ", " << extent[4] << " " << extent[5] << std::endl;
}

void Volume::readVoxels(VolumeReader* reader) {
	vtkImageData* imageData = reader->getImageData();
	void* voxels = imageData->GetScalarPointer();
}

void Volume::setSlicePlane(double planeAngle, double* normal, double* axis) {
	vtkPlane* plane = vtkPlane::New();

	plane->SetOrigin(sliceOrigin.data());

	plane->SetNormal(normal);
	this->GetProperty()->SetSliceFunction(plane);
}

void Volume::moveSliceOriginInDirection(double offset, double* direction) {
	// default slice origin is the center of the volume but it can change during the course of the run
	std::vector<double> newSliceOrigin(3);
	for (int i = 0; i < 3; ++i)
		newSliceOrigin[i] = sliceOrigin[i] + offset * direction[i];
	sliceOrigin = newSliceOrigin;
}

