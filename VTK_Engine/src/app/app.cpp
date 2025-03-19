#include "app/app.h"

#include "config.h"
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkRendererCollection.h>
#include <vtkContourValues.h>
#include <vtkNamedColors.h>
#include <vtkImageData.h>
#include <vtkPoints.h>
#include <vtkPlane.h>
#include <vtkTransform.h>
#include "interaction/frontendData.h"

App::App(int sceneWidth, int sceneHeight,
	int instanceWidth, int instanceHeight,
	int windowXPos, int windowYPos,
	int xpos, int ypos,
	float physicalHeight, float physicalDistance,
	float angleToRotate,
	std::string clientID, std::string& url, bool isHeadNode,
	int gpuIndex,
	int flexColumnNumber,
	std::string volumeFile) :Window(instanceWidth, instanceHeight, windowXPos, windowYPos, gpuIndex, false) {

	m_reader = new VolumeReader();
	m_reader->readVolume(volumeFile.c_str());

	m_mapper = new VolumeMapper();
	if (m_reader->fileType == VolumeReader::TIFF) // special handler for tiffs
		m_mapper->SetInputData(m_reader->getImageData()); 
	else m_mapper->SetInputConnection(m_reader->getOutputPort());

	m_property = new VolumeProperty();

	m_volume = new Volume();
	m_volume->SetMapper(m_mapper);
	m_volume->SetProperty(m_property);
	m_volume->setVolumeParameters(m_reader);
	m_volume->printSelf();
	double axis[] = { 0,1,0 }; double normal[] = { 0,0,1 };
	m_volume->setSlicePlane(angleToRotate, normal, axis);

	//m_volumeSlice = new VolumeSlicer(m_reader, m_volume);
	m_volumeOutline = new VolumeOutline(m_reader);

	//m_renderer->AddActor(m_volumeSlice->getSliceActor());
	m_renderer->AddActor(m_volumeOutline->getVolumeOutline());
	m_renderer->AddVolume(m_volume);

	std::vector<float> bgColor = FrontendData::getColor(FrontendData::defaultBackgroundColor);
	m_renderer->SetBackground(bgColor[0], bgColor[1], bgColor[2]);
	m_renderer->ResetCamera();

	m_camera = new Camera(sceneWidth, sceneHeight,
		instanceWidth, instanceHeight, 
		xpos, ypos, 
		physicalHeight, physicalDistance,
		angleToRotate);
	m_camera->setVolumeBounds(m_volume->getVolumeBounds());
	m_camera->resetCameraPosition();
	m_renderer->SetActiveCamera(m_camera->getCamera());

	m_isHeadNode = isHeadNode;
	m_clientID = clientID;
	m_interactor = new Interactor(m_renderer, m_mapper, m_property, m_volume, m_camera, m_volumeOutline, clientID, flexColumnNumber);
	m_interactor->setRenderCallback([this]() {this->render(); });

	m_client = new SocketClient(url, m_clientID, m_interactor, {m_volume->intensityRange[0], m_volume->intensityRange[1]});
}

void App::run() {
	render();
	m_client->startPolling();
}

App& App::getInstance(
	int sceneWidth, int sceneHeight,
	int instanceWidth, int instanceHeight,
	int xpos, int ypos,
	int windowXPos, int windowYPos,
	float physicalHeight, float physicalDistance, 
	float angleToRotate,
	std::string clientID, std::string& url, bool isHeadNode,
	int gpuIndex,
	int flexColumnNumber,
	std::string volumeFile) {
	static App instance(
		sceneWidth, sceneHeight, 
		instanceWidth, instanceHeight, 
		windowXPos, windowYPos,
		xpos, ypos,	
		physicalHeight, physicalDistance,
		angleToRotate,
		clientID, url, isHeadNode,
		gpuIndex,
		flexColumnNumber,
		volumeFile);
	return instance;
}

App::~App() {
	m_renderer->Delete();
	delete m_camera;
	delete m_reader;
	delete m_mapper;
	delete m_property;
	delete m_volume;
	delete m_client;
	delete m_interactor;
}
