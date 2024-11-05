#include "app/app.h"

#include "config.h"
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkRendererCollection.h>
#include <vtkContourValues.h>
#include <vtkNamedColors.h >
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>

float convertScale(float originalValue, float originalMin, float originalMax, float newMin, float newMax) {
	double originalRange = originalMax - originalMin;
	double newRange = newMax - newMin;
	return (((originalValue - originalMin) * newRange) / originalRange) + newMin;
}

App::App(int sceneWidth, int sceneHeight,
	int instanceWidth, int instanceHeight,
	int xpos, int ypos,
	float physicalHeight, float physicalDistance) :Window(instanceWidth, instanceHeight, xpos, ypos) {
	m_renderer = vtkRenderer::New();
	//m_interactor = new Interactor(m_window);

	/*m_camera = new Camera(
		sceneWidth, sceneHeight, 
		instanceWidth, instanceHeight, 
		xpos, ypos, 
		physicalHeight, physicalDistance);*/

	

	

	m_renderer->SetActiveCamera(m_camera);

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
	vtkNew<vtkNamedColors> colors;
	/*transferFunction->AddRGBPoint(Config::isoValues[0].value, 0, 0, 0);
	opacityFunction->AddPoint(Config::isoValues[0].value, 0.);

	transferFunction->AddRGBPoint(Config::isoValues[1].value, 0, 0, 0);
	opacityFunction->AddPoint(Config::isoValues[1].value, 0);*/
	for (auto isoValue : Config::isoValues) {
		transferFunction->AddRGBPoint(isoValue.value, isoValue.color[0], isoValue.color[1], isoValue.color[2]);
		transferFunction->AddRGBPoint(isoValue.value, isoValue.color[0], isoValue.color[1], isoValue.color[2]);
		opacityFunction->AddPoint(isoValue.value, isoValue.opacity);
	}
	transferFunction->AddRGBPoint(500.0, colors->GetColor3d("flesh").GetData()[0],
		colors->GetColor3d("flesh").GetData()[1],
		colors->GetColor3d("flesh").GetData()[2]);
	transferFunction->AddRGBPoint(1150.0, colors->GetColor3d("ivory").GetData()[0],
		colors->GetColor3d("ivory").GetData()[1],
		colors->GetColor3d("ivory").GetData()[2]);
	opacityFunction->AddPoint(500.0, 0.3);
	opacityFunction->AddPoint(1150.0, 0.6);

	m_property->SetColor(transferFunction);
	m_property->SetScalarOpacity(opacityFunction);

	// volume
	m_volume = new Volume();
	m_volume->SetMapper(m_mapper);
	m_volume->SetProperty(m_property);
	m_property->GetIsoSurfaceValues()->SetValue(0, 500.0);
	m_property->GetIsoSurfaceValues()->SetValue(1, 1150.0);

	m_renderer->AddVolume(m_volume);

	m_renderer->SetRenderWindow(m_window);
	m_renderer->SetBackground(colors->GetColor3d("cornflower").GetData());
	m_window->AddRenderer(m_renderer);
	vtkNew<vtkRenderWindowInteractor> interactor;
	vtkNew<vtkInteractorStyleTrackballCamera> style;
	m_interactor = interactor;
	interactor->SetInteractorStyle(style);
	m_window->SetInteractor(interactor);

	double* volumePosition = m_volume->GetBounds();
	


	m_camera = vtkExternalOpenGLCamera::New();
	float aspectRatio = (float)sceneWidth / (float)sceneHeight;
	float fov = 2 * atan(physicalHeight / (2 * -physicalDistance));

	float near = std::max(physicalDistance - 1.0f, 0.1f);
	float far = 1000.0f;

	float halfHeight = tan(fov / 2) * near;
	float halfWidth = halfHeight * aspectRatio;

	float left = convertScale(xpos, 0, sceneWidth, -halfWidth, halfWidth);
	float right = convertScale(xpos + instanceWidth, 0, sceneWidth, -halfWidth, halfWidth);
	float top = convertScale(sceneHeight - ypos, 0, sceneHeight, -halfHeight, halfHeight);
	float bottom = convertScale(sceneHeight - (ypos + instanceHeight), 0, sceneHeight, -halfHeight, halfHeight);

	vtkNew<vtkMatrix4x4> projectionMatrix;
	projectionMatrix->Zero();
	projectionMatrix->SetElement(0, 0, (2 * near) / (right - left));
	projectionMatrix->SetElement(0, 2, (right + left) / (right - left));
	projectionMatrix->SetElement(1, 1, (2 * near) / (top - bottom));
	projectionMatrix->SetElement(1, 2, (top + bottom) / (top - bottom));
	projectionMatrix->SetElement(2, 2, -(far + near) / (far - near));
	projectionMatrix->SetElement(2, 3, -2 * far * near / (far - near));
	projectionMatrix->SetElement(3, 2, -1.0);

	m_camera->SetUseExplicitProjectionTransformMatrix(true);
	m_camera->SetExplicitProjectionTransformMatrix(projectionMatrix);
	m_camera->SetPosition((volumePosition[0] + volumePosition[1]) / 2.0,
		(volumePosition[2] + volumePosition[3]) / 2.0,
		(volumePosition[4] + volumePosition[5]) / 2.0);
}

void App::run() {
	render();
	m_interactor->Start();
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