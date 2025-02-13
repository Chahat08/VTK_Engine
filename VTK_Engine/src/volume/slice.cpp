#include "volume/slice.h"
#include <vtkImageMapper3D.h>
#include <vtkImageReslice.h>
#include <vtkMatrix4x4.h>
#include <vtkPlaneSource.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>

VolumeSlicer::VolumeSlicer(VolumeReader* reader, Volume* volume) {
	//default black and white lookup table
	this->m_lookUpTable = vtkLookupTable::New();
	m_lookUpTable->SetTableRange(0, 2000);
	m_lookUpTable->SetSaturationRange(0, 0);
	m_lookUpTable->SetHueRange(0, 0);
	m_lookUpTable->SetValueRange(0, 1);
	m_lookUpTable->Build();

	this->m_volume = volume;

	mapImageToColors(reader);
	double normal[3] = { 1,0,0 };
	CreateSlicePlane(reader->getImageData(), m_lookUpTable, volume->origin, normal);
	imageActorSetup();
}

VolumeSlicer::VolumeSlicer(VolumeReader* reader, Volume* volume, vtkLookupTable* lookUpTable) {
	this->m_lookUpTable = lookUpTable;
	this->m_volume = volume;

	mapImageToColors(reader);
	imageActorSetup();
}

void VolumeSlicer::mapImageToColors(VolumeReader* reader){
	this->m_imageMap = vtkImageMapToColors::New();
	m_imageMap->SetInputConnection(reader->getOutputPort());
	m_imageMap->SetLookupTable(m_lookUpTable);
	m_imageMap->Update();
}

void VolumeSlicer::imageActorSetup() {
	this->m_imageActor = vtkImageActor::New();
	m_imageActor->GetMapper()->SetInputConnection(m_imageMap->GetOutputPort());
	m_imageActor->SetDisplayExtent(0, 255, 0, 255, 46, 46);
	m_imageActor->ForceOpaqueOn();
}

void VolumeSlicer::displaySliceAtWorldCoord(double* worldPos) {
	int startZ = static_cast<int>((worldPos[2] - m_volume->origin[2]) / m_volume->spacing[2]);

	if (startZ < m_volume->extent[4])
		startZ = m_volume->extent[4];
	if (startZ > m_volume->extent[5])
		startZ = m_volume->extent[5];

	setDisplayExtent(m_volume->extent[0], m_volume->extent[1],   
		m_volume->extent[2], m_volume->extent[3],   
		startZ, m_volume->extent[5]);
}

void VolumeSlicer::setDisplayExtent(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax) {
	m_imageActor->SetDisplayExtent(xmin, xmax, ymin, ymax, zmin, zmax);
}

vtkImageActor* VolumeSlicer::getSliceActor() {
	return m_imageActor;
}

void VolumeSlicer::CreateSlicePlane(vtkImageData* volume,
	vtkLookupTable* lookupTable,
	const double origin[3],
	const double normal[3])
{
    // Create the reslice axes matrix (this should match your reslice configuration)
    vtkSmartPointer<vtkMatrix4x4> resliceAxes =
        vtkSmartPointer<vtkMatrix4x4>::New();
    resliceAxes->Identity();

    // For example, create a 45 degree rotation about Z and translate to (50,50,30):
    double angle = 45.0 * vtkMath::Pi() / 180.0;
    double cosAngle = cos(angle);
    double sinAngle = sin(angle);

    // First column: X-axis of the slice
    resliceAxes->SetElement(0, 0, 1.0);
    resliceAxes->SetElement(1, 0, 0.0);
    resliceAxes->SetElement(2, 0, 0.0);

    // Second column: Y-axis of the slice
    resliceAxes->SetElement(0, 1, 0.0);
    resliceAxes->SetElement(1, 1, 1.0);
    resliceAxes->SetElement(2, 1, 0.0);

    // Third column: Normal of the slice (pointing outwards)
    resliceAxes->SetElement(0, 2, 0.0);
    resliceAxes->SetElement(1, 2, 0.0);
    resliceAxes->SetElement(2, 2, 1.0);

    // Fourth column: Translation (origin of the slice in world coordinates)
    resliceAxes->SetElement(0, 3, 0.0);
    resliceAxes->SetElement(1, 3, 0.0);
    resliceAxes->SetElement(2, 3, 0.0);

    // --- Create a quad plane in the slice's local (XY) coordinate system ---
    vtkSmartPointer<vtkPlaneSource> planeSource =
        vtkSmartPointer<vtkPlaneSource>::New();
    // Set the size of the plane (adjust these values as needed)

    std::vector<std::pair<double, double>> bounds = m_volume->getVolumeBounds();
    double planeSizeX = bounds[0].second;
    double planeSizeY = bounds[1].second;

    // Define a quad centered at the origin in the XY plane (z=0)
    planeSource->SetOrigin(-planeSizeX / 2.0, -planeSizeY / 2.0, 0.0);
    planeSource->SetPoint1(planeSizeX / 2.0, -planeSizeY / 2.0, 0.0);
    planeSource->SetPoint2(-planeSizeX / 2.0, planeSizeY / 2.0, 0.0);
    planeSource->Update();

    // --- Apply the reslice axes transformation to the plane ---
    vtkSmartPointer<vtkTransform> transform =
        vtkSmartPointer<vtkTransform>::New();
    transform->SetMatrix(resliceAxes);

    vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter =
        vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    transformFilter->SetInputConnection(planeSource->GetOutputPort());
    transformFilter->SetTransform(transform);
    transformFilter->Update();

    // --- Create a mapper and actor for the plane ---
    vtkSmartPointer<vtkPolyDataMapper> planeMapper =
        vtkSmartPointer<vtkPolyDataMapper>::New();
    planeMapper->SetInputConnection(transformFilter->GetOutputPort());

    m_planeActor = vtkActor::New();
    m_planeActor->SetMapper(planeMapper);
    m_planeActor->GetProperty()->SetColor(1.0, 0.0, 0.0);  // red color for visibility
    m_planeActor->GetProperty()->SetOpacity(0.5);
}

vtkActor* VolumeSlicer::getPlaneActor() {
    return m_planeActor;
}
