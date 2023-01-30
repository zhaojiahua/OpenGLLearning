#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <filesystem>

#include "Shader.h"
#include "ZCamera.h"
#include "ZDirectionLight.h"
#include "ZPointLight.h"
#include "ZSpotLight.h"
#include "ZMesh.h"

using namespace std;

//全局变量
//设置屏幕宽高
unsigned int screenWidth = 1400;
unsigned int screenHeight = 1000;
//计算deltaTime
float deltaTime = 0.0f;
float lastFrame = 0.0f;
//设置鼠标初始位置(鼠标的起始位置设置带屏幕的中心)
float lastX = screenWidth / 2, lastY = screenHeight / 2;
bool firstMouse = true;//第一次移动鼠标
//创建一个摄像机
ZCamera zcamera(45.0f, static_cast<float>(screenWidth) / static_cast<float>(screenHeight), 0.1f, 1000.0f);

//深度贴图分辨率
//unsigned int depthRes = 1024;
unsigned int depthRes = 2048;

//全局函数
//窗口大小发生变化时候的回调函数
void Framebuffer_size_callback(GLFWwindow* inWind, int inWidth, int inHeight) {
	glViewport(0, 20, inWidth, inHeight - 40);
}
//鼠标移动回调函数
void Mouse_move_callback(GLFWwindow* inWind, double xpos, double ypos) {
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xoffset = (xpos - lastX) * zcamera.sensitivity;
	float yoffset = (ypos - lastY) * zcamera.sensitivity;
	lastX = xpos;
	lastY = ypos;
	zcamera.SetCameraFront(xoffset, yoffset);
}
//鼠标滚轮回调函数
void Mouse_scroll_callback(GLFWwindow* inWind, double xoffset, double yoffset) {
	zcamera.SetCameraFov(yoffset);
}

//输入响应函数
void ProcessInput(GLFWwindow* inWind, Shader inshader) {
	if (glfwGetKey(inWind, GLFW_KEY_ESCAPE) == GLFW_PRESS) {//如果检测到ESC键被按下
		glfwSetWindowShouldClose(inWind, true);	//设置glfwWindowShouldClose为TRUE
	}
	if (glfwGetKey(inWind, GLFW_KEY_UP) == GLFW_PRESS) {//如果检测到up键被按下
		inshader.Use();
		float tempvalue = inshader.GetFloat("mixValue");
		tempvalue += 0.0002;
		if (tempvalue > 1.0f) tempvalue = 1.0f;
		inshader.SetFloat("mixValue", tempvalue);
	}
	if (glfwGetKey(inWind, GLFW_KEY_DOWN) == GLFW_PRESS) {//如果检测到down键被按下
		inshader.Use();
		float tempvalue = inshader.GetFloat("mixValue");
		tempvalue -= 0.0002;
		if (tempvalue < 0.0f) tempvalue = 0.0f;
		inshader.SetFloat("mixValue", tempvalue);
	}
	if (glfwGetKey(inWind, GLFW_KEY_W) == GLFW_PRESS) {//W键按下相机向前运动
		zcamera.MoveForward();
	}
	if (glfwGetKey(inWind, GLFW_KEY_S) == GLFW_PRESS) {//S键按下相机向后运动
		zcamera.MoveBackward();
	}
	if (glfwGetKey(inWind, GLFW_KEY_A) == GLFW_PRESS) {//A键按下相机向左运动
		zcamera.MoveLeft();
	}
	if (glfwGetKey(inWind, GLFW_KEY_D) == GLFW_PRESS) {//D键按下相机向右运动
		zcamera.MoveRight();
	}
	if (glfwGetKey(inWind, GLFW_KEY_SPACE) == GLFW_PRESS) {//空格键按下相机向上运动
		zcamera.MoveUpWard();
	}
	if (glfwGetKey(inWind, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {//shift+空格键按下相机向下运动
		zcamera.MoveDownWard();
	}
}
void ProcessInput(GLFWwindow* inWind) {
	if (glfwGetKey(inWind, GLFW_KEY_ESCAPE) == GLFW_PRESS) {//如果检测到ESC键被按下
		glfwSetWindowShouldClose(inWind, true);	//设置glfwWindowShouldClose为TRUE
	}
	if (glfwGetKey(inWind, GLFW_KEY_W) == GLFW_PRESS) {//W键按下相机向前运动
		zcamera.MoveForward();
	}
	if (glfwGetKey(inWind, GLFW_KEY_S) == GLFW_PRESS) {//S键按下相机向后运动
		zcamera.MoveBackward();
	}
	if (glfwGetKey(inWind, GLFW_KEY_A) == GLFW_PRESS) {//A键按下相机向左运动
		zcamera.MoveLeft();
	}
	if (glfwGetKey(inWind, GLFW_KEY_D) == GLFW_PRESS) {//D键按下相机向右运动
		zcamera.MoveRight();
	}
	if (glfwGetKey(inWind, GLFW_KEY_SPACE) == GLFW_PRESS) {//空格键按下相机向上运动
		zcamera.MoveUpWard();
	}
	if (glfwGetKey(inWind, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {//shift+空格键按下相机向下运动
		zcamera.MoveDownWard();
	}
}
//加载天空盒子
unsigned int LoadCubeMap(vector<string> textures_faces) {
	unsigned int texture_box;
	glGenTextures(1, &texture_box);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_box);
	//遍历纹理目标
	int texC_width, texC_height, texC_nrChannels;
	for (unsigned int i = 0; i < textures_faces.size(); i++) {
		unsigned char* tempdata = stbi_load(textures_faces[i].c_str(), &texC_width, &texC_height, &texC_nrChannels, 0);
		if (tempdata) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, texC_width, texC_height, 0, GL_RGB, GL_UNSIGNED_BYTE, tempdata);
		}
		else cout << "Cubemap texture failed to load at path:" << textures_faces[i] << endl;
		stbi_image_free(tempdata);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	return texture_box;
}