#ifndef INTERACTOR_H
#define INTERACTOR_H

#include <vtkRenderer.h>

#include "volume/property.h"
#include "interaction/frontendData.h"
#include "volume/volume.h"
#include "utils/json.hpp"

#include <functional>

class App;

class Interactor {
public:
	Interactor(vtkRenderer* renderer, VolumeProperty* property, Volume* volume);
	~Interactor();

	void handleServerMessage(const std::string& message) const;
private:
	friend class App;
	void setRenderCallback(const std::function<void()>& callback);

	VolumeProperty* m_property;
	Volume* m_volume;
	vtkRenderer* m_renderer;

	std::function<void()> m_renderCallback;

	nlohmann::json parseJson(const std::string& message) const;
	void reRender() const;

	void transferFunctionUpdate(const nlohmann::json& jsonData) const;
	void rendererBackgroundColorUpdate(const nlohmann::json& jsonData) const;
};

#endif