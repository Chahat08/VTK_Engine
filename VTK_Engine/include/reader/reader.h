#ifndef READER_H
#define READER_H

// TODO: add support for different file types

#include <vtkMetaImageReader.h>
#include <vtkStructuredPointsReader.h>

class VolumeReader {
public:
	VolumeReader();
	~VolumeReader();

	enum FileType {
		MetaImage,
		StructuredPoints,
		OpenVDB
	};

	void readVolume(const char* filename, FileType type = FileType::MetaImage);
	vtkAlgorithmOutput* getOutputPort();
	void clearVolume();

private:
	vtkMetaImageReader* m_metaImageReader{ nullptr };
	vtkStructuredPointsReader* m_structuredPointsReader{ nullptr };
};

#endif