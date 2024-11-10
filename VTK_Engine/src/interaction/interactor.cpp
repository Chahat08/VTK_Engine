#include "interaction/interactor.h"
#include "utils/json.hpp"
#include "interaction/frontendData.h"

Interactor::Interactor(VolumeProperty* property) {
	m_property = property;
}

void Interactor::setRenderCallback(const std::function<void()>& callback) {
	m_renderCallback = callback;
}

void Interactor::handleServerMessage(const std::string& message) const {
	std::cout << "Server: " << message << std::endl;

	if (!message.starts_with("CONNECT")) 
		transferFunctionUpdate(message);
}

void Interactor::transferFunctionUpdate(const std::string& message) const {
	nlohmann::json jsonData = nlohmann::json::parse(message);

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

	if (m_renderCallback)
		m_renderCallback();
}