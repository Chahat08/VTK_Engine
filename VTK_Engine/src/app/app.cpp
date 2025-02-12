#include "app/app.h"

#include "config.h"
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkRendererCollection.h>
#include <vtkContourValues.h>
#include <vtkNamedColors.h>
#include <vtkImageData.h>
#include <vtkPoints.h>
#include "interaction/frontendData.h"
#include "volume/slice.h"

App::App(int sceneWidth, int sceneHeight,
	int instanceWidth, int instanceHeight,
	int windowXPos, int windowYPos,
	int xpos, int ypos,
	float physicalHeight, float physicalDistance,
	float angleToRotate,
	std::string clientID, std::string& url, bool isHeadNode,
	int gpuIndex) :Window(instanceWidth, instanceHeight, windowXPos, windowYPos, gpuIndex, false) {

	m_reader = new VolumeReader();
	m_reader->readVolume(Config::readerConfig["fileName"].c_str(), VolumeReader::FileType::MetaImage);

	if (false) {
		/*vtkSmartPointer<vtkImageData> imageData =
			vtkImageData::SafeDownCast(m_reader->getImageData());

		m_reader->update();

		if (!imageData)
		{
			std::cerr << "Could not cast reader output to vtkImageData!\n";
			return;
		}

		double range[2];
		imageData->GetScalarRange(range);
		std::cout << "Intensity range: [" << range[0] << ", " << range[1] << "]\n";

		void* voxels = imageData->GetScalarPointer();
		if (!voxels)
		{
			std::cerr << "No voxel data in the image!\n";
			return;
		}*/
	}

	m_mapper = new VolumeMapper();
	m_mapper->SetInputConnection(m_reader->getOutputPort());

	m_property = new VolumeProperty();

	m_volume = new Volume();
	m_volume->SetMapper(m_mapper);
	m_volume->SetProperty(m_property);
	
	VolumeSlicer* m_volumeSlice = new VolumeSlicer(m_reader);
	m_renderer->AddActor(m_volumeSlice->getSliceActor());
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
	m_interactor = new Interactor(m_renderer, m_mapper, m_property, m_camera, clientID);
	m_interactor->setRenderCallback([this]() {this->render(); });

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
	int windowXPos, int windowYPos,
	float physicalHeight, float physicalDistance, 
	float angleToRotate,
	std::string clientID, std::string& url, bool isHeadNode,
	int gpuIndex) {
	static App instance(
		sceneWidth, sceneHeight, 
		instanceWidth, instanceHeight, 
		windowXPos, windowYPos,
		xpos, ypos,	
		physicalHeight, physicalDistance,
		angleToRotate,
		clientID, url, isHeadNode,
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
