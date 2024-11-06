#ifndef CAMERA_H
#define CAMERA_H

#include <vtkExternalOpenGLCamera.h>
#include <vtkMatrix4x4.h>

class Camera {
public:
	Camera(int sceneWidth, int sceneHeight,
		int instanceWidth, int instanceHeight,
		int xpos, int ypos,
		float physicalHeight, float physicalDistance);
	~Camera();

	void setPosition(double x, double y, double z);
	void setFocalPoint(double x, double y, double z);
	void setViewUp(double x, double y, double z);
	void setPitch(double pitch);
	void setYaw(double yaw);
	void setRoll(double roll);

	vtkExternalOpenGLCamera* getCamera();

private:
	vtkExternalOpenGLCamera* m_camera{ nullptr };

	int m_sceneWidth = 0, m_sceneHeight = 0;
	int m_instanceWidth = 0, m_instanceHeight = 0;
	int m_xpos = 0, m_ypos = 0;
	float m_physicalHeight = 0.0, m_physicalDistance = 0.0;

	float convertScale(float originalValue, float originalMin, float originalMax, float newMin, float newMax);
	vtkMatrix4x4* getInstanceProjectionMatrix();
};

#endif