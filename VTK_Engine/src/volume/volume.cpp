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
}

void Volume::readVoxels(VolumeReader* reader) {
	vtkImageData* imageData = reader->getImageData();
	void* voxels = imageData->GetScalarPointer();
}

void Volume::setSlicePlane(double planeAngle) {
	vtkPlane* plane = vtkPlane::New();

	std::vector<std::pair<double, double>> bounds = getVolumeBounds();
	double center[3] = {
		(bounds[0].first + bounds[0].second) / 2.0,
		(bounds[1].first + bounds[1].second) / 2.0,
		(bounds[2].first + bounds[2].second) / 2.0
	};
	plane->SetOrigin(center);

	double normal[3] = { 0.0,0.0,1.0 };
	vtkTransform* transform = vtkTransform::New();
	transform->Identity();
	transform->RotateY(planeAngle);
	transform->TransformVector(normal, normal);
	vtkMath::Normalize(normal);

	plane->SetNormal(normal);
	this->GetProperty()->SetSliceFunction(plane);
}