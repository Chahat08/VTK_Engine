#ifndef INTERACTOR_H
#define INTERACTOR_H

#include "volume/property.h"
#include "interaction/frontendData.h"
#include "volume/volume.h"
#include "volume/mapper.h"
#include "utils/json/simdjson.h"

#include <functional>

#include <vtkRenderer.h>

class App;

class Interactor {
public:
	Interactor(vtkRenderer* renderer, VolumeMapper* mapper, VolumeProperty* property, Volume* volume);
	~Interactor();

	void handleServerMessage(const std::string& message) const;
private:
	friend class App;

	VolumeMapper* m_mapper;
	VolumeProperty* m_property;
	Volume* m_volume;
	vtkRenderer* m_renderer;

	std::function<void()> m_renderCallback;
	void setRenderCallback(const std::function<void()>& callback);
	void reRender() const;

	void parseJson(const std::string& message) const;

	// renderer updates
	void rendererBackgroundColorUpdate(simdjson::ondemand::object& jsonData) const;

	// volume mapper updates
	void autoSampleDistancesUpdate(simdjson::ondemand::object& jsonData) const;
	void sampleDistanceUpdate(simdjson::ondemand::object& jsonData) const;
	void blendModeUpdate(simdjson::ondemand::object& jsonData) const;

	// volume property updates
	void transferFunctionOpacityUpdate(simdjson::ondemand::object& jsonData) const;
	void transferFunctionColorUpdate(simdjson::ondemand::object& jsonData) const;
	void shadingUpdate(simdjson::ondemand::object& jsonData) const;
	void interpolationTypeUpdate(simdjson::ondemand::object& jsonData) const;
	void addIsovalueUpdate(simdjson::ondemand::object& jsonData) const;

	
};

#endif