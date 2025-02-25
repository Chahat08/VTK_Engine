#ifndef READER_H
#define READER_H

// TODO: add support for different file types

#include <vtkMetaImageReader.h>
#include <vtkTIFFReader.h>
#include <vtkOMETIFFReader.h>
#include <vtkImageData.h>
#include <vtkStructuredPointsReader.h>
#include <vtkImageCast.h>
#include <vtkImageLuminance.h>
#include <vtkImageAppendComponents.h>

class VolumeReader {
public:
	VolumeReader();
	~VolumeReader();

	enum FileType {
		MetaImage,
		TIFF,
		StructuredPoints
	};

	FileType fileType;
	void readVolume(const char* filename);
	vtkAlgorithmOutput* getOutputPort();
	vtkImageData* getImageData();
	void update();
	void clearVolume();

private:
	vtkMetaImageReader* m_metaImageReader{ nullptr };
	vtkTIFFReader* m_TIFFReader{ nullptr };
	vtkImageData* m_TIFFMagImage{ nullptr };
	vtkStructuredPointsReader* m_structuredPointsReader{ nullptr };

	void checkFileType(std::string fileName);
};

#endif