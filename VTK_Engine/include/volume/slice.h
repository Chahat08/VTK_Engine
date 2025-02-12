#include <vtkLookupTable.h>
#include <vtkImageMapToColors.h>
#include <vtkImageActor.h>
#include "reader/reader.h"

class VolumeSlicer {
public:
	VolumeSlicer(VolumeReader* reader, vtkLookupTable* lookUpTable);
	VolumeSlicer(VolumeReader* reader);

	vtkImageActor* getSliceActor();

private:
	vtkLookupTable* m_lookUpTable;
	vtkImageMapToColors* m_imageMap;
	vtkImageActor* m_imageActor;

	void mapImageToColors(VolumeReader* reader);
	void imageActorSetup();
};