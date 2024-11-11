#include "app/app.h"

#include "config.h"
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkRendererCollection.h>
#include <vtkContourValues.h>
#include <vtkNamedColors.h >

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
	

	double iso1 = 500.0;
	double iso2 = 1150.0;

	m_reader = new VolumeReader();
	m_reader->readVolume(Config::readerConfig["fileName"].c_str(), VolumeReader::FileType::MetaImage);

	vtkNew<vtkNamedColors> colors;

	m_mapper = new VolumeMapper();
	m_mapper->SetInputConnection(m_reader->getOutputPort());
	m_mapper->AutoAdjustSampleDistancesOff();
	m_mapper->SetSampleDistance(0.5);
	m_mapper->SetBlendModeToComposite();

	vtkNew<vtkPiecewiseFunction> scalarOpacity;
	scalarOpacity->AddPoint(iso1, .3);
	scalarOpacity->AddPoint(iso2, 0.6);

	m_property = new VolumeProperty();

	vtkNew<vtkVolume> volume;
	volume->SetMapper(m_mapper);
	volume->SetProperty(m_property);
	
	m_renderer->AddVolume(volume);
	m_renderer->SetBackground(colors->GetColor3d("cornflower").GetData());
	m_renderer->ResetCamera();

	float instancePositionX = xpos, instancePositionY = ypos;

	m_window->SetSize(instanceWidth, instanceHeight);
	m_window->SetPosition(instancePositionX, instancePositionY);
	m_window->BordersOff();
	m_window->AddRenderer(m_renderer);

	//m_property->GetIsoSurfaceValues()->SetValue(0, iso1);
	//m_property->GetIsoSurfaceValues()->SetValue(1, iso2);

	m_camera = new Camera(sceneWidth, sceneHeight,
		instanceWidth, instanceHeight, 
		xpos, ypos, 
		physicalHeight, physicalDistance);

	m_renderer->SetActiveCamera(m_camera->getCamera());
	double* volumePosition = volume->GetBounds();
	m_camera->setPosition(((volumePosition[0] + volumePosition[1]) / 2.0) - 500.0,
		((volumePosition[2] + volumePosition[3]) / 2.0) - 80.0,
		((volumePosition[4] + volumePosition[5]) / 2.0) - 300.0);

	m_interactor = new Interactor(m_property);
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