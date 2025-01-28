#ifndef READER_H
#define READER_H

// TODO: add support for different file types

#include <vtkMetaImageReader.h>
#include <vtkImageData.h>

class VolumeReader {
public:
	VolumeReader();
	~VolumeReader();

	enum FileType {
		MetaImage,
	};

	void readVolume(const char* filename, FileType type = FileType::MetaImage);
	vtkAlgorithmOutput* getOutputPort();
	vtkImageData* getImageData();
	void update();
	void clearVolume();

private:
	vtkMetaImageReader* m_reader{ nullptr };
};

#endif