#include "camera/camera.h"
#include "interaction/frontendData.h"
#include <vtkTransform.h>
#include <vtkMath.h>
#include <vtkQuaternion.h>

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
	m_cameraRight = new double[3];
	resetCameraPosition();
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
	double* viewUp = new double[3] { x, y, z };
	vtkMath::Normalize(viewUp);
	m_camera->SetViewUp(viewUp);
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

void Camera::setVolumeBounds(std::vector<std::pair<double, double>> bounds) {
	m_volumeBounds = bounds;
}

void Camera::resetCameraPosition() {
	if (m_volumeBounds.empty()) {
		m_camera->SetPosition(0, 0, 0);
		setFocalPoint(0, 0, 0);
		setViewUp(0, 1, 0);
		m_cameraRight[0] = 1;
		m_cameraRight[1] = 0;
		m_cameraRight[2] = 0;
	}
	else {
		m_camera->SetPosition(
			(m_volumeBounds[0].first + m_volumeBounds[0].second) / 2.0,
			(m_volumeBounds[1].first + m_volumeBounds[1].second) / 2.0,
			4*m_volumeBounds[2].second 
		);
		setFocalPoint(
			(m_volumeBounds[0].first + m_volumeBounds[0].second) / 2.0,
			(m_volumeBounds[1].first + m_volumeBounds[1].second) / 2.0,
			(m_volumeBounds[2].first + m_volumeBounds[2].second) / 2.0
		);
		setViewUp(0, 1, 0);
	}
}

void Camera::rotateCamera(double deltaX, double deltaY) {
	double xRotationScale = 10.0;
    double yRotationScale = 10.0;

	double viewUp[3], viewDir[3];
	m_camera->GetViewUp(viewUp);

	double pos[3], focal[3];
	m_camera->GetPosition(pos);
	m_camera->GetFocalPoint(focal);
	for (int i = 0; i < 3; i++) {
		viewDir[i] = focal[i] - pos[i];
	}
	vtkMath::Normalize(viewDir);

	double angle = vtkMath::DegreesFromRadians(
		acos(vtkMath::Dot(viewDir, viewUp))
	);

	const double MIN_ANGLE = 10.0;  
	const double MAX_ANGLE = 170.0; 

	if ((angle < MIN_ANGLE && deltaY > 0) ||
		(angle > MAX_ANGLE && deltaY < 0)) {
		deltaY = 0;
	}
    
    double yAngle = deltaX * xRotationScale;
    double xAngle = deltaY * yRotationScale;
    
    std::vector<double> currentPosition = {
        m_camera->GetPosition()[0] - m_camera->GetFocalPoint()[0],
        m_camera->GetPosition()[1] - m_camera->GetFocalPoint()[1],
        m_camera->GetPosition()[2] - m_camera->GetFocalPoint()[2],
    };
    
    vtkNew<vtkTransform> transform;
    transform->PostMultiply();
    transform->RotateWXYZ(yAngle, m_camera->GetViewUp()[0], m_camera->GetViewUp()[1], m_camera->GetViewUp()[2]);
    
    double newRightVector[3];
    transform->TransformVector(m_cameraRight, newRightVector);
    transform->RotateWXYZ(xAngle, newRightVector[0], newRightVector[1], newRightVector[2]);
    
    double newPoint[3];
    transform->TransformPoint(currentPosition.data(), newPoint);
    
    m_camera->SetPosition(
        newPoint[0] + m_camera->GetFocalPoint()[0],
        newPoint[1] + m_camera->GetFocalPoint()[1],
        newPoint[2] + m_camera->GetFocalPoint()[2]
    );
    
	m_camera->Modified();
}



void Camera::zoomCamera(double zoomFactor) {
	double* position = m_camera->GetPosition();
	double* focalPoint = m_camera->GetFocalPoint();

	double* cameraDirection = new double[3]{
		position[0] - focalPoint[0],
		position[1] - focalPoint[1],
		position[2] - focalPoint[2]
	};

	vtkMath::Normalize(cameraDirection);

	m_camera->SetPosition(
		position[0] + zoomFactor*cameraDirection[0],
		position[1] + zoomFactor*cameraDirection[1],
		position[2] + zoomFactor*cameraDirection[2]
	);

	m_camera->Modified();

}

void Camera::moveCamera(double deltaX, double deltaY, double deltaZ) {
	double* position = m_camera->GetPosition();
	double* focalPoint = m_camera->GetFocalPoint();
	double* viewUp = m_camera->GetViewUp();

	double* cameraDirection = new double[3] {
		position[0] - focalPoint[0],
			position[1] - focalPoint[1],
			position[2] - focalPoint[2]
		};
	vtkMath::Normalize(cameraDirection);

	double* cameraRight = new double[3];
	vtkMath::Cross(cameraDirection, viewUp, cameraRight);
	vtkMath::Normalize(cameraRight);

	double* cameraUp = new double[3];
	vtkMath::Cross(cameraRight, cameraDirection, cameraUp);
	vtkMath::Normalize(cameraUp);

	m_camera->SetPosition(
		position[0] + deltaX * cameraRight[0] + deltaY * cameraUp[0] + deltaZ * cameraDirection[0],
		position[1] + deltaX * cameraRight[1] + deltaY * cameraUp[1] + deltaZ * cameraDirection[1],
		position[2] + deltaX * cameraRight[2] + deltaY * cameraUp[2] + deltaZ * cameraDirection[2]
	);

	m_camera->SetFocalPoint(
		focalPoint[0] + deltaX * cameraRight[0] + deltaY * cameraUp[0] + deltaZ * cameraDirection[0],
		focalPoint[1] + deltaX * cameraRight[1] + deltaY * cameraUp[1] + deltaZ * cameraDirection[1],
		focalPoint[2] + deltaX * cameraRight[2] + deltaY * cameraUp[2] + deltaZ * cameraDirection[2]
	);

	m_camera->Modified();
}