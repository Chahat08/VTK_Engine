#ifndef INTERACTOR_H
#define INTERACTOR_H

#include "volume/property.h"
#include "interaction/frontendData.h"
#include <functional>

class App;

class Interactor {
public:
	Interactor(VolumeProperty* property);
	~Interactor();

	void handleServerMessage(const std::string& message) const;
private:
	friend class App;
	void setRenderCallback(const std::function<void()>& callback);

	VolumeProperty* m_property;
	std::function<void()> m_renderCallback;

	void transferFunctionUpdate(const std::string& message) const;
};

#endif