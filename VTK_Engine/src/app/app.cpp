#include "app/app.h"

#include "config.h"
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkRendererCollection.h>
#include <vtkContourValues.h>
#include <vtkNamedColors.h >
#include "interaction/frontendData.h"

App::App(int sceneWidth, int sceneHeight,
	int instanceWidth, int instanceHeight,
	int windowXPos, int windowYPos,
	int xpos, int ypos,
	float physicalHeight, float physicalDistance,
	float angleToRotate,
	std::string clientID, std::string& url,
	int gpuIndex) :Window(instanceWidth, instanceHeight, windowXPos, windowYPos, gpuIndex, false) {

	m_reader = new VolumeReader();
	m_reader->readVolume(Config::readerConfig["fileName"].c_str(), VolumeReader::FileType::StructuredPoints);

	m_mapper = new VolumeMapper();
	m_mapper->SetInputConnection(m_reader->getOutputPort());
	std::cout << "here1";
	m_property = new VolumeProperty();

	m_volume = new Volume();
	m_volume->SetMapper(m_mapper);
	m_volume->SetProperty(m_property);
	std::cout << "here2";

	
	m_renderer->AddVolume(m_volume);
	std::vector<float> bgColor = FrontendData::getColor(FrontendData::defaultBackgroundColor);
	m_renderer->SetBackground(bgColor[0], bgColor[1], bgColor[2]);
	m_renderer->ResetCamera();
	std::cout << "here3";


	m_camera = new Camera(sceneWidth, sceneHeight,
		instanceWidth, instanceHeight, 
		xpos, ypos, 
		physicalHeight, physicalDistance,
		angleToRotate);
	m_camera->setVolumeBounds(m_volume->getVolumeBounds());
	m_camera->resetCameraPosition();
	m_renderer->SetActiveCamera(m_camera->getCamera());
	std::cout << "here4";


	m_clientID = clientID;
	m_interactor = new Interactor(m_renderer, m_mapper, m_property, m_camera, clientID);
	m_interactor->setRenderCallback([this]() {this->render(); });

	m_client = new SocketClient(url, m_clientID, m_interactor);
	std::cout << "here5";

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
	std::string clientID, std::string& url,
	int gpuIndex) {
	static App instance(
		sceneWidth, sceneHeight, 
		instanceWidth, instanceHeight, 
		windowXPos, windowYPos,
		xpos, ypos,	
		physicalHeight, physicalDistance,
		angleToRotate,
		clientID, url,
		gpuIndex);
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
