#include "reader/reader.h"
#include <vtkNew.h>
#include <vtkImageCast.h>
#include <vtkPointData.h>
#include <vtkDataArray.h>



VolumeReader::VolumeReader() {
}

void VolumeReader::readVolume(const char* filename) {
	checkFileType(filename);

	switch (fileType) {

	case FileType::MetaImage:
		m_metaImageReader = vtkMetaImageReader::New();
		m_metaImageReader->SetFileName(filename);
		m_metaImageReader->Update();
		std::cout << "NUM COM: " << m_metaImageReader->GetNumberOfScalarComponents() << std::endl;

		break;

	case FileType::TIFF:
		m_TIFFReader = vtkTIFFReader::New();
		m_TIFFReader->SetFileDimensionality(3);
		m_TIFFReader->SetNumberOfScalarComponents(3);
		m_TIFFReader->SetFileName(filename);
		m_TIFFReader->Update();

		// TODO: More flexibility in handling tiffs related to num of scalar components and their interpretation

		// For now, magnitude (euclidean norm of components)is what we will render

		vtkSmartPointer<vtkImageCast> castFilter = vtkSmartPointer<vtkImageCast>::New();
		castFilter->SetInputConnection(m_TIFFReader->GetOutputPort());
		castFilter->SetOutputScalarTypeToDouble();
		castFilter->Update();

		vtkImageData* imageData = castFilter->GetOutput();
		int dims[3];
		imageData->GetDimensions(dims);
		int numComponents = imageData->GetNumberOfScalarComponents();

		if (numComponents < 3)
		{
			std::cerr << "TIFF does not have at least 3 components." << std::endl;
			return;
		}

		vtkIdType numPoints = imageData->GetNumberOfPoints();

		m_TIFFMagImage = vtkImageData::New();
		m_TIFFMagImage->SetDimensions(dims);
		m_TIFFMagImage->AllocateScalars(VTK_DOUBLE, 1);

		for (vtkIdType i = 0; i < numPoints; i++)
		{
			double rgb[3];
			imageData->GetPointData()->GetScalars()->GetTuple(i, rgb);
			double magnitude = std::sqrt(rgb[0] * rgb[0] + rgb[1] * rgb[1] + rgb[2] * rgb[2]);
			m_TIFFMagImage->GetPointData()->GetScalars()->SetTuple1(i, magnitude);
		}
		
		break;
	}
}

void VolumeReader::checkFileType(std::string fileName) {
	if (std::string(fileName).ends_with(".mhd"))
		fileType = FileType::MetaImage;

	else if (std::string(fileName).ends_with(".tiff") || std::string(fileName).ends_with(".tif"))
		fileType = FileType::TIFF;

	else if (std::string(fileName).ends_with(".vtk"))
		fileType = FileType::StructuredPoints;
}

vtkAlgorithmOutput* VolumeReader::getOutputPort() {
	if (m_metaImageReader) 
		return m_metaImageReader->GetOutputPort();

	if (m_TIFFReader)
		return m_TIFFReader->GetOutputPort();

	if (m_structuredPointsReader)
		return m_structuredPointsReader->GetOutputPort();

	return nullptr;
}

vtkImageData* VolumeReader::getImageData() {
	if (m_metaImageReader)
		return m_metaImageReader->GetOutput();

	if (m_TIFFMagImage)
		return m_TIFFMagImage;

	return nullptr;
}

void VolumeReader::update() {
	if (m_metaImageReader)
		m_metaImageReader->Update();

	else if (m_TIFFReader)
		m_TIFFReader->Update();

	else if (m_structuredPointsReader)
		m_structuredPointsReader->Update();
}

void VolumeReader::clearVolume() {
	if (m_metaImageReader)
		m_metaImageReader->RemoveAllInputs();

	else if (m_TIFFReader)
		m_TIFFReader->RemoveAllInputs();

	else if (m_structuredPointsReader)
		m_structuredPointsReader->RemoveAllInputs();
}

VolumeReader::~VolumeReader() {
	if (m_metaImageReader)
		 m_metaImageReader->Delete();

	if (m_TIFFReader)
		 m_TIFFReader->Delete();

	if (m_structuredPointsReader)
		 m_structuredPointsReader->Delete();
}