#ifndef INTERACTOR_H
#define INTERACTOR_H

#include "volume/property.h"
#include "interaction/frontendData.h"
#include "volume/volume.h"
#include "utils/json/simdjson.h"

#include <functional>

#include <vtkRenderer.h>

class App;

class Interactor {
public:
	Interactor(vtkRenderer* renderer, VolumeProperty* property, Volume* volume);
	~Interactor();

	void handleServerMessage(const std::string& message) const;
private:
	friend class App;

	VolumeProperty* m_property;
	Volume* m_volume;
	vtkRenderer* m_renderer;

	std::function<void()> m_renderCallback;
	void setRenderCallback(const std::function<void()>& callback);
	void reRender() const;

	void parseJson(const std::string& message) const;

	void transferFunctionOpacityUpdate(simdjson::ondemand::object& jsonData) const;
	void transferFunctionColorUpdate(simdjson::ondemand::object& jsonData) const;
	void shadingUpdate(simdjson::ondemand::object& jsonData) const;
	void interpolationTypeUpdate(simdjson::ondemand::object& jsonData) const;

	void rendererBackgroundColorUpdate(simdjson::ondemand::object& jsonData) const;
	
};

#endif