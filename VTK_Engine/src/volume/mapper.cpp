#include "volume/mapper.h"
#include <vtkImageData.h>
#include <vtkPoints.h>

VolumeMapper::VolumeMapper() {
	this->SetAutoAdjustSampleDistances(FrontendData::defaultAutoSampling);
	this->SetSampleDistance(FrontendData::defaultSampleDistance);
	this->setBlendMode(FrontendData::defaultBlendMode);
}

void VolumeMapper::setBlendMode(std::string mode) {
	if (mode == "Isosurface") {
		this->SetBlendModeToIsoSurface();
	}
	else if (mode == "Composite") {
		this->SetBlendModeToComposite();
	}
	else if (mode == "Max Intensity") {
		this->SetBlendModeToMaximumIntensity();
	}
	else if (mode == "Min Intensity") {
		this->SetBlendModeToMinimumIntensity();
	}
	else if (mode == "Average Intensity") {
		this->SetBlendModeToAverageIntensity();
	}
	else if (mode == "Additive") {
		this->SetBlendModeToAdditive();
	}
	else if (mode == "Slice") {
		this->SetBlendModeToSlice();
	}
	else {
		this->SetBlendModeToComposite();
	}
}

VolumeMapper::~VolumeMapper() {
	this->Delete();
}