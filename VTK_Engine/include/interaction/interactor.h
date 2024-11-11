#ifndef INTERACTOR_H
#define INTERACTOR_H

#include "volume/property.h"
#include "interaction/frontendData.h"
#include "volume/volume.h"

#include <functional>

class App;

class Interactor {
public:
	Interactor(VolumeProperty* property, Volume* volume);
	~Interactor();

	void handleServerMessage(const std::string& message) const;
private:
	friend class App;
	void setRenderCallback(const std::function<void()>& callback);

	VolumeProperty* m_property;
	Volume* m_volume;
	std::function<void()> m_renderCallback;

	void transferFunctionUpdate(const std::string& message) const;
};

#endif