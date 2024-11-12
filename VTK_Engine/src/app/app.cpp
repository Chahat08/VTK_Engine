#include "app/app.h"

#include "config.h"
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkRendererCollection.h>
#include <vtkContourValues.h>
#include <vtkNamedColors.h >
#include "interaction/frontendData.h"

float convertScale(float originalValue, float originalMin, float originalMax, float newMin, float newMax) {
	double originalRange = originalMax - originalMin;
	double newRange = newMax - newMin;
	return (((originalValue - originalMin) * newRange) / originalRange) + newMin;
}

App::App(int sceneWidth, int sceneHeight,
	int instanceWidth, int instanceHeight,
	int xpos, int ypos,
	float physicalHeight, float physicalDistance,
	int clientID, std::string& url) :Window(instanceWidth, instanceHeight, xpos, ypos) {

	m_reader = new VolumeReader();
	m_reader->readVolume(Config::readerConfig["fileName"].c_str(), VolumeReader::FileType::MetaImage);

	vtkNew<vtkNamedColors> colors;

	m_mapper = new VolumeMapper();
	m_mapper->SetInputConnection(m_reader->getOutputPort());

	m_property = new VolumeProperty();

	m_volume = new Volume();
	m_volume->SetMapper(m_mapper);
	m_volume->SetProperty(m_property);
	
	m_renderer->AddVolume(m_volume);
	std::vector<float> bgColor = FrontendData::getColor(FrontendData::defaultBackgroundColor);
	m_renderer->SetBackground(bgColor[0], bgColor[1], bgColor[2]);
	m_renderer->ResetCamera();

	float instancePositionX = xpos, instancePositionY = ypos;

	m_window->SetSize(instanceWidth, instanceHeight);
	m_window->SetPosition(instancePositionX, instancePositionY);
	m_window->BordersOff();
	m_window->AddRenderer(m_renderer);

	m_camera = new Camera(sceneWidth, sceneHeight,
		instanceWidth, instanceHeight, 
		xpos, ypos, 
		physicalHeight, physicalDistance);
	m_camera->setVolumeBounds(m_volume->getVolumeBounds());
	m_camera->resetCameraPosition();
	m_renderer->SetActiveCamera(m_camera->getCamera());

	m_interactor = new Interactor(m_renderer, m_mapper, m_property, m_camera);
	m_interactor->setRenderCallback([this]() {this->render(); });
	m_clientID = clientID;
	m_client = new SocketClient(url, m_clientID, m_interactor);
}

void App::run() {
	render();
	m_client->startPolling();
}

App& App::getInstance(
	int sceneWidth, int sceneHeight,
	int instanceWidth, int instanceHeight,
	int xpos, int ypos,
	float physicalHeight, float physicalDistance, 
	int clientID, std::string& url) {
	static App instance(
		sceneWidth, sceneHeight, 
		instanceWidth, instanceHeight, 
		xpos, ypos,	
		physicalHeight, physicalDistance,
		clientID, url);
	return instance;
}

App::~App() {
	m_renderer->Delete();
}