#include "app/app.h"

#include "config.h"
#include "vtkColorTransferFunction.h"
#include "vtkPiecewiseFunction.h"

App::App(int sceneWidth, int sceneHeight,
	int instanceWidth, int instanceHeight,
	int xpos, int ypos,
	float physicalHeight, float physicalDistance) :Window(instanceWidth, instanceHeight, xpos, ypos) {
	m_renderer = vtkRenderer::New();
	m_camera = new Camera(
		sceneWidth, sceneHeight, 
		instanceWidth, instanceHeight, 
		xpos, ypos, 
		physicalHeight, physicalDistance);
	m_renderer->SetActiveCamera(m_camera->getCamera());

	// volume reader
	m_reader = new VolumeReader();
	if (Config::readerConfig["fileType"] == "MetaImage")
		m_reader->readVolume(Config::readerConfig["fileName"].c_str(), VolumeReader::FileType::MetaImage);

	// volume mapper
	m_mapper = new VolumeMapper();

	if(Config::mapperConfig["AutoAdjustSampleDistances"]=="off")
		m_mapper->SetAutoAdjustSampleDistances(false);
	m_mapper->SetSampleDistance(std::stof(Config::mapperConfig["SampleDistance"]));
	m_mapper->SetInputConnection(m_reader->getOutputPort());
	m_mapper->setBlendMode(Config::mapperConfig["BlendMode"]);

	// volume property
	m_property = new VolumeProperty();
	if (Config::volumeProperties["Shade"] == "On")
		m_property->SetShade(true);
	m_property->setInterpolationType(Config::volumeProperties["InterpolationType"]);

	vtkNew<vtkColorTransferFunction> transferFunction;
	vtkNew<vtkPiecewiseFunction> opacityFunction;
	transferFunction->RemoveAllPoints();
	for (auto isoValue : Config::isoValues) {
		transferFunction->AddRGBPoint(isoValue.value, isoValue.color[0], isoValue.color[1], isoValue.color[2]);
		opacityFunction->AddPoint(isoValue.value, isoValue.opacity);
	}
	m_property->SetColor(transferFunction);
	m_property->SetScalarOpacity(opacityFunction);

	// volume
	m_volume = new Volume();
	m_volume->SetMapper(m_mapper);
	m_volume->SetProperty(m_property);

	m_renderer->AddVolume(m_volume);
	m_renderer->ResetCamera();

	m_window->AddRenderer(m_renderer);

	double* volumePosition = m_volume->GetBounds();
	std::cout << volumePosition[0] << " " << volumePosition[1] << " " << volumePosition[2] << " " << volumePosition[3] << " " << volumePosition[4] << " " << volumePosition[5] << std::endl;
	m_camera->setPosition((volumePosition[0] + volumePosition[1]) / 2.0,
		(volumePosition[2] + volumePosition[3]) / 2.0,
		(volumePosition[4] + volumePosition[5]) / 2.0);
}

void App::run() {
	render();
}

App& App::getInstance(
	int sceneWidth, int sceneHeight,
	int instanceWidth, int instanceHeight,
	int xpos, int ypos,
	float physicalHeight, float physicalDistance) {
	static App instance(sceneWidth, sceneHeight, instanceWidth, instanceHeight, xpos, ypos,	physicalHeight, physicalDistance);
	return instance;
}

App::~App() {
	m_renderer->Delete();
}