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
	double iso1 = 500.0;
	double iso2 = 1150.0;
	vtkNew<vtkMetaImageReader> reader;
	reader->SetFileName("FullHead.mhd");

	vtkNew<vtkNamedColors> colors;

	vtkNew<vtkOpenGLGPUVolumeRayCastMapper> mapper;
	mapper->SetInputConnection(reader->GetOutputPort());
	mapper->AutoAdjustSampleDistancesOff();
	mapper->SetSampleDistance(0.5);
	mapper->SetBlendModeToIsoSurface();

	vtkNew<vtkColorTransferFunction> colorTransferFunction;
	colorTransferFunction->RemoveAllPoints();
	colorTransferFunction->AddRGBPoint(iso2,
		colors->GetColor3d("ivory").GetData()[0],
		colors->GetColor3d("ivory").GetData()[1],
		colors->GetColor3d("ivory").GetData()[2]);
	colorTransferFunction->AddRGBPoint(iso1,
		colors->GetColor3d("flesh").GetData()[0],
		colors->GetColor3d("flesh").GetData()[1],
		colors->GetColor3d("flesh").GetData()[2]);

	vtkNew<vtkPiecewiseFunction> scalarOpacity;
	scalarOpacity->AddPoint(iso1, .3);
	scalarOpacity->AddPoint(iso2, 0.6);

	vtkNew<vtkVolumeProperty> volumeProperty;
	volumeProperty->ShadeOn();
	volumeProperty->SetInterpolationTypeToLinear();
	volumeProperty->SetColor(colorTransferFunction);
	volumeProperty->SetScalarOpacity(scalarOpacity);

	vtkNew<vtkVolume> volume;
	volume->SetMapper(mapper);
	volume->SetProperty(volumeProperty);
	std::cout << volume->GetMinXBound() << " " << volume->GetMaxXBound() << " " << volume->GetMinYBound() << " " << volume->GetMaxYBound() << " " << volume->GetMinZBound() << " " << volume->GetMaxZBound() << "\n";
	//volume->SetPosition(0, 0, 5);

	//vtkNew<vtkRenderer> renderer;
	m_renderer->AddVolume(volume);
	m_renderer->SetBackground(colors->GetColor3d("cornflower").GetData());
	m_renderer->ResetCamera();

	float instancePositionX = xpos, instancePositionY = ypos;

	//vtkNew<vtkRenderWindow> renderWindow;
	m_window->SetSize(instanceWidth, instanceHeight);
	m_window->SetPosition(instancePositionX, instancePositionY);
	m_window->BordersOff();
	m_window->AddRenderer(m_renderer);
	m_window->SetWindowName("RayCastIsosurface");

	vtkNew<vtkInteractorStyleTrackballCamera> style;

	vtkNew<vtkRenderWindowInteractor> interactor;
	interactor->SetRenderWindow(m_window);
	interactor->SetInteractorStyle(style);

	// Add some contour values to draw iso surfaces
	volumeProperty->GetIsoSurfaceValues()->SetValue(0, iso1);
	volumeProperty->GetIsoSurfaceValues()->SetValue(1, iso2);

	vtkNew<vtkExternalOpenGLCamera> camera;
	m_renderer->SetActiveCamera(camera);

	float aspectRatio = (float)sceneWidth / (float)sceneHeight;
	float fov = 2 * atan(physicalHeight / (2 * -physicalDistance));

	float near = std::max(physicalDistance - 1.0f, 0.1f);
	float far = 1000.0f;

	float halfHeight = tan(fov / 2) * near;
	float halfWidth = halfHeight * aspectRatio;

	float left = convertScale(instancePositionX, 0, sceneWidth, -halfWidth, halfWidth);
	float right = convertScale(instancePositionX + instanceWidth, 0, sceneWidth, -halfWidth, halfWidth);
	float top = convertScale(sceneHeight - instancePositionY, 0, sceneHeight, -halfHeight, halfHeight);
	float bottom = convertScale(sceneHeight - (instancePositionY + instanceHeight), 0, sceneHeight, -halfHeight, halfHeight);

	vtkNew<vtkMatrix4x4> projectionMatrix;
	projectionMatrix->Zero();
	projectionMatrix->SetElement(0, 0, (2 * near) / (right - left));
	projectionMatrix->SetElement(0, 2, (right + left) / (right - left));
	projectionMatrix->SetElement(1, 1, (2 * near) / (top - bottom));
	projectionMatrix->SetElement(1, 2, (top + bottom) / (top - bottom));
	projectionMatrix->SetElement(2, 2, -(far + near) / (far - near));
	projectionMatrix->SetElement(2, 3, -2 * far * near / (far - near));
	projectionMatrix->SetElement(3, 2, -1.0);

	camera->SetUseExplicitProjectionTransformMatrix(true);
	camera->SetExplicitProjectionTransformMatrix(projectionMatrix);
	double* volumePosition = volume->GetBounds();
	camera->SetPosition((volumePosition[0] + volumePosition[1]) / 2.0,
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