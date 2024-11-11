#include "interaction/interactor.h"


Interactor::Interactor(vtkRenderer* renderer, VolumeProperty* property, Volume* volume) {
	m_property = property;
	m_volume = volume;
	m_renderer = renderer;
}

void Interactor::setRenderCallback(const std::function<void()>& callback) {
	m_renderCallback = callback;
}

void Interactor::reRender() const {
	if (m_renderCallback)
		m_renderCallback();
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