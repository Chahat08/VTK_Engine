#include "volume/slice.h"
#include <vtkImageMapper3D.h>


VolumeSlicer::VolumeSlicer(VolumeReader* reader) {
	//default black and white lookup table
	this->m_lookUpTable = vtkLookupTable::New();
	m_lookUpTable->SetTableRange(0, 2000);
	m_lookUpTable->SetSaturationRange(0, 0);
	m_lookUpTable->SetHueRange(0, 0);
	m_lookUpTable->SetValueRange(0, 1);
	m_lookUpTable->Build();

	mapImageToColors(reader);
	imageActorSetup();
}

VolumeSlicer::VolumeSlicer(VolumeReader* reader, vtkLookupTable* lookUpTable) {
	this->m_lookUpTable = lookUpTable;

	mapImageToColors(reader);
	imageActorSetup();
}

void VolumeSlicer::mapImageToColors(VolumeReader* reader){
	this->m_imageMap = vtkImageMapToColors::New();
	m_imageMap->SetInputConnection(reader->getOutputPort());
	m_imageMap->SetLookupTable(m_lookUpTable);
	m_imageMap->Update();
}

void VolumeSlicer::imageActorSetup() {
	this->m_imageActor = vtkImageActor::New();
	m_imageActor->GetMapper()->SetInputConnection(m_imageMap->GetOutputPort());
	m_imageActor->SetDisplayExtent(128, 128, 0, 255, 0, 92);
	m_imageActor->ForceOpaqueOn();
}

vtkImageActor* VolumeSlicer::getSliceActor() {
	return m_imageActor;
}
