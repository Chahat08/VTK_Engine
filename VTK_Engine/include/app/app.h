#ifndef APP_H
#define APP_H

#include "app/window.h"
#include "camera/camera.h"
#include "reader/reader.h"
#include "volume/mapper.h"
#include "volume/property.h"
#include "volume/volume.h"
#include "interaction/client.h"
#include "interaction/interactor.h"
#include "volume/slice.h"
#include "volume/outline.h"

class App :private Window {
public:
	static App& getInstance(
		int sceneWidth, int sceneHeight, 
		int instanceWidth, int instanceHeight,
		int windowXPos, int windowYPos,
		int xpos, int ypos, 
		float physicalHeight, float physicalDistance,
		float angleToRotate,
		std::string clientID, std::string& url, bool isHeadNode,
		int gpuIndex
	);
	void run();
private:
	App(int sceneWidth, int sceneHeight,
		int instanceWidth, int instanceHeight,
		int xpos, int ypos,
		int windowXPos, int windowYPos,
		float physicalHeight, float physicalDistance,
		float angleToRotate,
		std::string clientID, std::string& url, bool isHeadNode,
		int gpuIndex);
	~App();
	App(const App&) = delete;
	App& operator=(const App&) = delete;

	std::string m_clientID;
	bool m_isHeadNode;

	Camera* m_camera;
	VolumeReader* m_reader;
	VolumeMapper* m_mapper;
	VolumeProperty* m_property;
	Volume* m_volume;
	SocketClient* m_client;
	Interactor* m_interactor;
	VolumeSlicer* m_volumeSlice;
	VolumeOutline* m_volumeOutline;
};

#endif