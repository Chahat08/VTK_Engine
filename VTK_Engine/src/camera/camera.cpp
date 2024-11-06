#include "camera/camera.h"

#include <vtkNew.h>

float Camera::convertScale(float originalValue, float originalMin, float originalMax, float newMin, float newMax) {
	double originalRange = originalMax - originalMin;
	double newRange = newMax - newMin;
	return (((originalValue - originalMin) * newRange) / originalRange) + newMin;
}

vtkMatrix4x4* Camera::getInstanceProjectionMatrix() {
	float aspectRatio = (float)m_sceneWidth / (float)m_sceneHeight;
	float fov = 2 * atan(m_physicalHeight / (2 * -m_physicalDistance));

	float near = std::max(m_physicalDistance - 1.0f, 0.1f);
	float far = 1000.0f;

	float halfHeight = tan(fov / 2) * near;
	float halfWidth = halfHeight * aspectRatio;

	float left = convertScale(m_xpos, 0, m_sceneWidth, -halfWidth, halfWidth);
	float right = convertScale(m_xpos + m_instanceWidth, 0, m_sceneWidth, -halfWidth, halfWidth);
	float top = convertScale(m_sceneHeight - m_ypos, 0, m_sceneHeight, -halfHeight, halfHeight);
	float bottom = convertScale(m_sceneHeight - (m_ypos + m_instanceHeight), 0, m_sceneHeight, -halfHeight, halfHeight);

	vtkMatrix4x4* projectionMatrix = vtkMatrix4x4::New();
	projectionMatrix->Zero();
	projectionMatrix->SetElement(0, 0, (2 * near) / (right - left));
	projectionMatrix->SetElement(0, 2, (right + left) / (right - left));
	projectionMatrix->SetElement(1, 1, (2 * near) / (top - bottom));
	projectionMatrix->SetElement(1, 2, (top + bottom) / (top - bottom));
	projectionMatrix->SetElement(2, 2, -(far + near) / (far - near));
	projectionMatrix->SetElement(2, 3, -2 * far * near / (far - near));
	projectionMatrix->SetElement(3, 2, -1.0);

	return projectionMatrix;
}

Camera::Camera(int sceneWidth, int sceneHeight,
	int instanceWidth, int instanceHeight,
	int xpos, int ypos,
	float physicalHeight, float physicalDistance) {

	m_sceneWidth = sceneWidth;
	m_sceneHeight = sceneHeight;
	m_instanceWidth = instanceWidth;
	m_instanceHeight = instanceHeight;
	m_xpos = xpos;
	m_ypos = ypos;
	m_physicalHeight = physicalHeight;
	m_physicalDistance = physicalDistance;

	m_camera = vtkExternalOpenGLCamera::New();

	m_camera->SetUseExplicitProjectionTransformMatrix(true);
	m_camera->SetExplicitProjectionTransformMatrix(getInstanceProjectionMatrix());
}

Camera::~Camera() {
	m_camera->Delete();
}

void Camera::setPosition(double x, double y, double z) {
	m_camera->SetPosition(x, y, z);
}

void Camera::setFocalPoint(double x, double y, double z) {
	m_camera->SetFocalPoint(x, y, z);
}

void Camera::setViewUp(double x, double y, double z) {
	m_camera->SetViewUp(x, y, z);
}

void Camera::setPitch(double pitch) {
	m_camera->Pitch(pitch);
}

void Camera::setYaw(double yaw) {
	m_camera->Yaw(yaw);
}

void Camera::setRoll(double roll) {
	m_camera->Roll(roll);
}

vtkExternalOpenGLCamera* Camera::getCamera() {
	return m_camera;
}