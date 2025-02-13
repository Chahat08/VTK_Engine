#ifndef SLICE_H
#define SLICE_H

#include <vtkLookupTable.h>
#include <vtkImageMapToColors.h>
#include <vtkImageActor.h>
#include <vtkActor.h>
#include "reader/reader.h"
#include "volume/volume.h"

class VolumeSlicer {
public:
	VolumeSlicer(VolumeReader* reader, Volume* volume, vtkLookupTable* lookUpTable);
	VolumeSlicer(VolumeReader* reader, Volume* volume);

	vtkImageActor* getSliceActor();
	vtkActor* getPlaneActor();
	void setDisplayExtent(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);
	void displaySliceAtWorldCoord(double* worldPos);

private:

	vtkLookupTable* m_lookUpTable;
	vtkImageMapToColors* m_imageMap;
	vtkImageActor* m_imageActor;
	Volume* m_volume;
	vtkActor* m_planeActor;

	void mapImageToColors(VolumeReader* reader);
	void imageActorSetup();
	void CreateSlicePlane(vtkImageData* volume,
		vtkLookupTable* lookupTable,
		const double origin[3],
		const double normal[3]);
};

#endif