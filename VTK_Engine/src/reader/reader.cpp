#include "reader/reader.h"

VolumeReader::VolumeReader() {
}

void VolumeReader::readVolume(const char* filename, FileType type) {
	switch (type) {
	case FileType::MetaImage:
		m_metaImageReader = vtkMetaImageReader::New();
		m_metaImageReader->SetFileName(filename);
		m_metaImageReader->Update();
		break;

	case FileType::StructuredPoints:
		m_structuredPointsReader = vtkStructuredPointsReader::New();
		m_structuredPointsReader->SetFileName(filename);
		m_structuredPointsReader->Update();
		std::cout << "here";
		break;
	}
}

vtkAlgorithmOutput* VolumeReader::getOutputPort() {
	return m_structuredPointsReader->GetOutputPort();
}

void VolumeReader::clearVolume() {
	m_structuredPointsReader->RemoveAllInputs();
}

VolumeReader::~VolumeReader() {
	m_structuredPointsReader->Delete();
}