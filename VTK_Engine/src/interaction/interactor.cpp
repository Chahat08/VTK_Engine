#include "interaction/interactor.h"


Interactor::Interactor(vtkRenderer* renderer, VolumeMapper* mapper, VolumeProperty* property, Camera* camera, std::string clientID) {
	m_property = property;
	m_camera = camera;
	m_renderer = renderer;
	m_mapper = mapper;
	m_clientID = clientID;
}

Interactor::~Interactor() {
	
}

void Interactor::setRenderCallback(const std::function<void()>& callback) {
	m_renderCallback = callback;
}

void Interactor::reRender() const {
	if (m_renderCallback)
		m_renderCallback();
}

void Interactor::setTerminateCallback(const std::function<void()>& callback) {
	m_terminateCallback = callback;
}

void Interactor::terminate() const {
	if (m_terminateCallback)
		m_terminateCallback();
}

void Interactor::parseJson(const std::string& message) const {

	simdjson::ondemand::parser parser;
	simdjson::padded_string json = simdjson::padded_string(message);
	simdjson::ondemand::document doc = parser.iterate(json);

	simdjson::ondemand::object obj = doc.get_object();

	if (obj["bgColor"].error() == simdjson::SUCCESS)
		rendererBackgroundColorUpdate(obj);

	else if (obj["controlPoints"].error() == simdjson::SUCCESS)
		transferFunctionOpacityUpdate(obj);

	else if (obj["colorStops"].error() == simdjson::SUCCESS)
		transferFunctionColorUpdate(obj);

	else if (obj["shading"].error() == simdjson::SUCCESS)
		shadingUpdate(obj);

	else if (obj["interpolationType"].error() == simdjson::SUCCESS)
		interpolationTypeUpdate(obj);

	else if (obj["autosampleDistances"].error() == simdjson::SUCCESS)
		autoSampleDistancesUpdate(obj);

	else if (obj["sampleDistance"].error() == simdjson::SUCCESS)
		sampleDistanceUpdate(obj);

	else if (obj["blendMode"].error() == simdjson::SUCCESS)
		blendModeUpdate(obj);

	else if (obj["isosurfaceValues"].error() == simdjson::SUCCESS)
		addIsovalueUpdate(obj);

	else if (obj["cameraDrag"].error() == simdjson::SUCCESS)
		cameraPositionUpdate(obj);

	else if (obj["cameraZoom"].error() == simdjson::SUCCESS)
		cameraZoomUpdate(obj);

	else if (obj["joystick"].error() == simdjson::SUCCESS || obj["direction"].error() == simdjson::SUCCESS)
		cameraJoystickUpdates(obj);

	else if (obj["resetCamera"].error() == simdjson::SUCCESS)
		cameraResetUpdate();

	else if (obj["arcballSpeed"].error() == simdjson::SUCCESS)
		cameraArcballSpeedUpdate(obj);

	else if (obj["freeCameraSpeed"].error() == simdjson::SUCCESS)
		cameraFreeCameraSpeedUpdate(obj);

	else if (obj["terminate_all"].error() == simdjson::SUCCESS)
		terminate();
	
	else if(obj["terminate_client"].error()==simdjson::SUCCESS)
		terminateAppUpdate(obj);
}

void Interactor::handleServerMessage(const std::string& message) const {
	std::cout << "Server: " << message << std::endl;

	if (!message.starts_with("CONNECT"))
		parseJson(message);
}

void Interactor::rendererBackgroundColorUpdate(simdjson::ondemand::object& jsonData) const {
	std::string_view color = jsonData["bgColor"].get_string();
	std::vector<float> bgColor = FrontendData::getColor(std::string(color));

	m_renderer->SetBackground(bgColor[0], bgColor[1], bgColor[2]);
	m_renderer->Modified();

	reRender();
}

void Interactor::transferFunctionOpacityUpdate(simdjson::ondemand::object& jsonData) const {
	std::vector<FrontendData::OpacityControlPoint> controlPoints;
	auto controlPointsArray = jsonData["controlPoints"].get_array();

	for (simdjson::ondemand::object point : controlPointsArray) {
		double value = 0.0, opacity = 0.0;

		simdjson::error_code value_error = point["value"].get(value);
		simdjson::error_code opacity_error = point["opacity"].get(opacity);

		if (value_error == simdjson::SUCCESS && opacity_error == simdjson::SUCCESS)
			controlPoints.push_back({ value, opacity });
	}

	m_property->setOpacityPoints(controlPoints);
	reRender();
}

void Interactor::transferFunctionColorUpdate(simdjson::ondemand::object& jsonData) const {
	std::vector<FrontendData::ColorGradientStopPoint> colorStops;
	auto colorStopsArray = jsonData["colorStops"].get_array();

	for (simdjson::ondemand::object point : colorStopsArray) {
		double position = 0.0;
		std::string_view color;

		simdjson::error_code position_error = point["position"].get(position);
		simdjson::error_code color_error = point["color"].get_string(color);

		if (position_error == simdjson::SUCCESS && color_error == simdjson::SUCCESS)
			colorStops.push_back({ position, std::string(color) });
	}

	m_property->setColorPoints(colorStops);
	reRender();
}

void Interactor::shadingUpdate(simdjson::ondemand::object& jsonData) const {
	bool shading = false;
	simdjson::error_code shading_error = jsonData["shading"].get(shading);

	if (shading_error == simdjson::SUCCESS) {
		m_property->SetShade(shading);
		m_property->Modified();
		reRender();
	}
}

void Interactor::interpolationTypeUpdate(simdjson::ondemand::object& jsonData) const {
	std::string_view interpolationType;
	simdjson::error_code interpolation_error = jsonData["interpolationType"].get_string(interpolationType);

	if (interpolation_error == simdjson::SUCCESS) {
		m_property->setInterpolationType(std::string(interpolationType));
		m_property->Modified();
		reRender();
	}
}

void Interactor::autoSampleDistancesUpdate(simdjson::ondemand::object& jsonData) const {
	bool autoSampling = false;
	simdjson::error_code autoSampling_error = jsonData["autosampleDistances"].get(autoSampling);

	if (autoSampling_error == simdjson::SUCCESS) {
		m_mapper->SetAutoAdjustSampleDistances(autoSampling);
		m_mapper->Modified();
		reRender();
	}
}

void Interactor::sampleDistanceUpdate(simdjson::ondemand::object& jsonData) const {
	float sampleDistance = 0.0;
	simdjson::error_code sampleDistance_error = jsonData["sampleDistance"].get(sampleDistance);

	if (sampleDistance_error == simdjson::SUCCESS) {
		m_mapper->SetSampleDistance(sampleDistance);
		m_mapper->Modified();
		reRender();
	}
}

void Interactor::blendModeUpdate(simdjson::ondemand::object& jsonData) const {
	std::string_view blendMode;
	simdjson::error_code blendMode_error = jsonData["blendMode"].get_string(blendMode);

	if (blendMode_error == simdjson::SUCCESS) {
		m_mapper->setBlendMode(std::string(blendMode));
		m_mapper->Modified();
		reRender();
	}
}

void Interactor::addIsovalueUpdate(simdjson::ondemand::object& jsonData) const {
	std::vector<float> isovalues;
	auto isovaluesArray = jsonData["isosurfaceValues"].get_array();

	for (simdjson::ondemand::value value : isovaluesArray) {
		float isovalue = 0.0;
		simdjson::error_code isovalue_error = value.get(isovalue);

		if (isovalue_error == simdjson::SUCCESS)
			isovalues.push_back(isovalue);
	}

	m_property->setIsovalues(isovalues);
	reRender();
}

void Interactor::cameraPositionUpdate(simdjson::ondemand::object& jsonData) const {

	simdjson::ondemand::object cameraDrag = jsonData["cameraDrag"];

	int deltaX = 0, deltaY = 0;
	simdjson::error_code x_error = cameraDrag["deltaX"].get(deltaX);
	simdjson::error_code y_error = cameraDrag["deltaY"].get(deltaY);

	if (x_error == simdjson::SUCCESS && y_error == simdjson::SUCCESS) {
		m_camera->arcballMove(deltaX, deltaY);
		reRender();
	}
	m_camera->printSelf();

}

void Interactor::cameraZoomUpdate(simdjson::ondemand::object& jsonData) const {
	double zoom = 0.0;
	simdjson::error_code zoom_error = jsonData["cameraZoom"].get(zoom);

	if (zoom_error == simdjson::SUCCESS) {
		m_camera->arcballZoom(zoom);
		reRender();
	}
	m_camera->printSelf();

}

void Interactor::cameraJoystickUpdates(simdjson::ondemand::object& jsonData) const {
	std::string_view joystick;
	simdjson::error_code blendMode_error = jsonData["joystick"].get_string(joystick);

	if (blendMode_error == simdjson::SUCCESS) {
		double deltaX = 0, deltaZ = 0;
		simdjson::error_code x_error = jsonData["x"].get(deltaX);
		simdjson::error_code y_error = jsonData["y"].get(deltaZ);

		m_camera->freeCameraMove(deltaX, 0, -deltaZ);
	}

	else {
		std::string_view dir;
		simdjson::error_code direction = jsonData["direction"].get(dir);

		if (dir == "up")
			m_camera->freeCameraMove(0, -1, 0);
		else if (dir == "down")
			m_camera->freeCameraMove(0, 1, 0);
	}
	m_camera->printSelf();
	reRender();
}

void Interactor::cameraResetUpdate() const {
	m_camera->resetCameraPosition();
	reRender();
}

void Interactor::cameraArcballSpeedUpdate(simdjson::ondemand::object& jsonData) const {
	float speed = 0.0;
	simdjson::error_code speed_error = jsonData["arcballSpeed"].get(speed);

	if (speed_error == simdjson::SUCCESS) 
		m_camera->setArcBallSpeed(speed);
}

void Interactor::cameraFreeCameraSpeedUpdate(simdjson::ondemand::object& jsonData) const {
	float speed = 0.0;
	simdjson::error_code speed_error = jsonData["freeCameraSpeed"].get(speed);

	if (speed_error == simdjson::SUCCESS) 
		m_camera->setFreeCameraSpeed(speed);
}

void Interactor::terminateAppUpdate(simdjson::ondemand::object& jsonData) const {
	std::string_view client;
	simdjson::error_code terminate_error = jsonData["terminate_client"].get(client);

	if (terminate_error == simdjson::SUCCESS)
		if(client==m_clientID)
			terminate();
}