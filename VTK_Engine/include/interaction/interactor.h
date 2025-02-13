#ifndef INTERACTOR_H
#define INTERACTOR_H

#include "volume/property.h"
#include "interaction/frontendData.h"
#include "volume/mapper.h"
#include "camera/camera.h"
#include "utils/json/simdjson.h"
#include "volume/outline.h"

#include <functional>

#include <vtkRenderer.h>

class App;

class Interactor {
public:
	Interactor(vtkRenderer* renderer, VolumeMapper* mapper, VolumeProperty* property, Camera* camera, VolumeOutline* outline, std::string clientID);
	~Interactor();
	
	void handleServerMessage(const std::string& message) const;
private:
	friend class App;
	friend class SocketClient;

	VolumeMapper* m_mapper;
	VolumeProperty* m_property;
	vtkRenderer* m_renderer;
	Camera* m_camera;
	VolumeOutline* m_volumeOutline;

	std::function<void()> m_renderCallback;
	void setRenderCallback(const std::function<void()>& callback);
	void reRender() const;

	std::string m_clientID;
	std::function<void()> m_terminateCallback;
	void setTerminateCallback(const std::function<void()>& callback);
	void terminate() const;

	void parseJson(const std::string& message) const;

	// app updates
	void terminateAppUpdate(simdjson::ondemand::object& jsonData) const;

	// renderer updates
	void rendererBackgroundColorUpdate(simdjson::ondemand::object& jsonData) const;

	// volume outline updates
	void outlineUpdate(simdjson::ondemand::object& jsonData) const;
	
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

	// camera updates
	void cameraPositionUpdate(simdjson::ondemand::object& jsonData) const;
	void cameraZoomUpdate(simdjson::ondemand::object& jsonData) const;
	void cameraJoystickUpdates(simdjson::ondemand::object& jsonData) const;
	void cameraResetUpdate() const;
	void cameraArcballSpeedUpdate(simdjson::ondemand::object& jsonData) const;
	void cameraFreeCameraSpeedUpdate(simdjson::ondemand::object& jsonData) const;
};

#endif