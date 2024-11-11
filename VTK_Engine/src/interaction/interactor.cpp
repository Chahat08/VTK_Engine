#include "interaction/interactor.h"
#include "interaction/frontendData.h"

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

void Interactor::handleServerMessage(const std::string& message) const {
	std::cout << "Server: " << message << std::endl;

	if (!message.starts_with("CONNECT")) 
		transferFunctionUpdate(message);
}

nlohmann::json Interactor::parseJson(const std::string& message) const {
	nlohmann::json jsonData = nlohmann::json::parse(message);
	if (jsonData.contains("controlPoints"))
		transferFunctionUpdate(jsonData);
	else if (jsonData.contains("bgColor"))
		rendererBackgroundColorUpdate(jsonData);
}

void Interactor::rendererBackgroundColorUpdate(const nlohmann::json& jsonData) const {
	float* bgColor = FrontendData::getColor(jsonData["bgColor"]["color"]).data();
	m_renderer->SetBackground(bgColor[0], bgColor[1], bgColor[2]);

	reRender();
}

void Interactor::transferFunctionUpdate(const nlohmann::json& jsonData) const {
	std::vector<FrontendData::OpacityControlPoint> controlPoints;
	std::vector<FrontendData::ColorGradientStopPoint> colorPoints;

	for (const auto& point : jsonData["controlPoints"])
		controlPoints.push_back(FrontendData::OpacityControlPoint{
				point["value"].get<double>(),
				point["opacity"].get<double>()
			});

	for (const auto& stopPoint : jsonData["colorStops"])
		colorPoints.push_back({
				stopPoint["position"].get<double>(),
				stopPoint["color"].get<std::string>()
			});

	m_property->setColorPoints(colorPoints);
	m_property->setOpacityPoints(controlPoints);

	reRender();
}