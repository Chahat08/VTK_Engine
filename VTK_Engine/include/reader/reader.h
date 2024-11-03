#ifndef READER_H
#define READER_H

// TODO: add support for different file types

#include <vtkMetaImageReader.h>

class VolumeReader {
public:
	VolumeReader();
	~VolumeReader();

	enum FileType {
		MetaImage,
	};

	void readVolume(const char* filename, FileType type = FileType::MetaImage);
	vtkAlgorithmOutput* getOutputPort();
	void clearVolume();

private:
	vtkMetaImageReader* m_reader{ nullptr };
};

#endif