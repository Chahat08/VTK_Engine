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

void VolumeReader::clearVolume() {
	m_reader->RemoveAllInputs();
}

VolumeReader::~VolumeReader() {
	m_reader->Delete();
}