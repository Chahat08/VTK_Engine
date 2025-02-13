#ifndef OUTLINE_H
#define OUTLINE_H

#include "reader/reader.h"

#include <vtkOutlineFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkActor.h>

class VolumeOutline {
public:
	VolumeOutline(VolumeReader* reader);

	vtkActor* getVolumeOutline();

private:
	vtkActor* m_outlineActor;
};

#endif