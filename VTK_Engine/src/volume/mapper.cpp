#include "volume/mapper.h"

VolumeMapper::VolumeMapper() {
	this->AutoAdjustSampleDistancesOn();
	//m_mapper->SetSampleDistance(0.5);
	this->SetBlendModeToComposite();
}

void VolumeMapper::setBlendMode(std::string mode) {
	if (mode == "IsoSurface") {
		this->SetBlendModeToIsoSurface();
	}
	else if (mode == "Composite") {
		this->SetBlendModeToComposite();
	}
	else if (mode == "MaximumIntensity") {
		this->SetBlendModeToMaximumIntensity();
	}
	else if (mode == "MinimumIntensity") {
		this->SetBlendModeToMinimumIntensity();
	}
	else if (mode == "AverageIntensity") {
		this->SetBlendModeToAverageIntensity();
	}
	else if (mode == "Additive") {
		this->SetBlendModeToAdditive();
	}
	else {
		this->SetBlendModeToIsoSurface();
	}
}

VolumeMapper::~VolumeMapper() {
	this->Delete();
}