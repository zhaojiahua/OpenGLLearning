#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ZCamera
{
public:
	ZCamera();
	ZCamera(float inFov, float inRatio, float inNear, float inFar);

	glm::mat4 perspectiveMatrix = glm::mat4(1.0f);
	glm::mat4 viewMatrix = glm::mat4(1.0f);
	glm::vec3 cameraPos = { 0.0f,0.0f,150.0f };
	glm::vec3 cameraUp = { 0.0f,1.0f,0.0f };
	glm::vec3 cameraFront = { 0.0f,0.0f,-1.0f };
	float yaw = 0.0f;
	float pitch = 0.0f;

	float cameraMoveSpeed = 0.005f;
	float sensitivity = 0.02f;	//鼠标灵敏度
	float fov = 45.0f;//初始相机视角为45度
	float ratio = 4.0f / 3.0f;
	float near = 0.1f;
	float far = 500.0f;

	//根据已有的Pitch和yaw的值计算并返回cameraFront
	glm::vec3 GetCameraFront();
	//根据已有的相机位置等属性计算viewMatrix并返回
	glm::mat4 GetCameraViewMatrix();
	//根据已有的相机属性计算perspectiveMatrix并返回
	glm::mat4 GetCameraperspectiveMatrix();
	//传入Pitch和yaw的offset值设置cameraFront
	void SetCameraFront(const float& inxoffset, const float& inyoffset);
	//传入视张角的变化量设置相机的视张角
	void SetCameraFov(const float& inoffset);

	//相机向前移动
	void MoveForward();
	//相机向后移动
	void MoveBackward();
	//相机向左移动
	void MoveLeft();
	//相机向右移动
	void MoveRight();
	//相机向上移动
	void MoveUpWard();
	//相机向下移动
	void MoveDownWard();
};

