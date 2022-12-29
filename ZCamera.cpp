#include "ZCamera.h"

ZCamera::ZCamera(){}

ZCamera::ZCamera(float inFov, float inRatio, float inNear, float inFar)
{
	fov = inFov;
	ratio = inRatio;
	near = inNear;
	far = inFar;
}

glm::vec3 ZCamera::GetCameraFront()
{
	glm::vec3 tempFront;
	tempFront.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	tempFront.y = sin(glm::radians(pitch));
	tempFront.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	return tempFront;
}

glm::mat4 ZCamera::GetCameraViewMatrix()
{
	viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	return viewMatrix;
}

glm::mat4 ZCamera::GetCameraperspectiveMatrix()
{
	perspectiveMatrix = glm::perspective(glm::radians(fov), ratio, near, far);
	return perspectiveMatrix;
}

void ZCamera::SetCameraFront(const float& inxoffset, const float& inyoffset)
{
	yaw += inxoffset;
	pitch -= inyoffset;
	if (pitch > 89.0f)pitch = 89.0f;
	if (pitch < -89.0f)pitch = -89.0f;
	cameraFront = GetCameraFront();
}

void ZCamera::SetCameraFov(const float& inoffset)
{
	if (fov >= 1.0f && fov <= 60.0f) fov -= inoffset;
	if (fov < 1.0f)fov = 1.0f;
	if (fov > 60.0f)fov = 60.0f;
}

void ZCamera::MoveForward()
{
	cameraPos += cameraFront * cameraMoveSpeed;
}

void ZCamera::MoveBackward()
{
	cameraPos -= cameraFront * cameraMoveSpeed;
}

void ZCamera::MoveLeft()
{
	cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraMoveSpeed;
}

void ZCamera::MoveRight()
{
	cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraMoveSpeed;
}
