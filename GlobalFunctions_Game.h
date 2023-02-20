#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <filesystem>
#include <random>

#include "Shader.h"
#include "Texture2D.h"
#include "ResourceManager.h"
#include "Game.h"

#include <irrKlang.h>

using namespace std;

//全局变量
//设置屏幕宽高
GLuint screenWidth = 1000;
GLuint screenHeight = 800;
//创建一个游戏实例,这个游戏实例也是全局变量
Game BreakoutGame(screenWidth, screenHeight);

//全局函数
//窗口大小发生变化时候的回调函数
void Framebuffer_size_callback(GLFWwindow* inWind, int inWidth, int inHeight) {
	glViewport(0, 20, inWidth, inHeight - 40);
}
//鼠标移动回调函数
void Mouse_move_callback(GLFWwindow* inWind, double xpos, double ypos) {

}
//鼠标滚轮回调函数
void Mouse_scroll_callback(GLFWwindow* inWind, double xoffset, double yoffset) {
	
}

//输入响应函数
void Key_callback(GLFWwindow* inWind, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action==GLFW_PRESS) {//如果检测到ESC键被按下
		glfwSetWindowShouldClose(inWind, GL_TRUE);	//设置glfwWindowShouldClose为TRUE
	}
	if (key > 0 && key < 1024) {//把所有的按键存储在一列表里面,谁被按下谁就变TRUE,否则就是FALS
		if (action == GLFW_PRESS) BreakoutGame.Keys[key] = GL_TRUE;
		else if (action == GLFW_RELEASE)BreakoutGame.Keys[key] = GL_FALSE;
	}
}

//加速差值函数(把线性分布函数曲线下压成抛物线)
GLfloat AccelerateLerp(GLfloat a, GLfloat b, GLfloat f) {
	return a + f * (b - a);
}
