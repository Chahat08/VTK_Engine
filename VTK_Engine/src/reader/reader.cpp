#include "reader/reader.h"

VolumeReader::VolumeReader() {
}

void VolumeReader::readVolume(const char* filename, FileType type) {
	switch (type) {
	case FileType::MetaImage:
		m_reader = vtkMetaImageReader::New();
		m_reader->SetFileName(filename);
		break;
	}
}

vtkAlgorithmOutput* VolumeReader::getOutputPort() {
	return m_reader->GetOutputPort();
}

vtkImageData* VolumeReader::getImageData() {
	return m_reader->GetOutput();
}

void VolumeReader::update() {
	m_reader->Update();
}

void VolumeReader::clearVolume() {
	m_reader->RemoveAllInputs();
}

VolumeReader::~VolumeReader() {
	m_reader->Delete();
}