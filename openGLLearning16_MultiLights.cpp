#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "Shader.h"
#include "ZCamera.h"

using namespace std;

//设置屏幕宽高
unsigned int screenWidth = 1000;
unsigned int screenHeight = 800;
//计算deltaTime
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//创建一个摄像机
ZCamera zcamera(45.0f, static_cast<float>(screenWidth) / static_cast<float>(screenHeight), 0.1f, 500.0f);

//配置平行光源(红色调)
glm::vec3 g_dirlightColor = { 1.0f,0.3f,0.2f };
glm::vec3 g_dirlightDirection = { -0.2f,-1.0f,-0.3f };
//配置点光源(蓝色调)
glm::vec3 g_pointlightColor= { 0.2f,0.25f,1.0f };
glm::vec3 g_pointlightPositions[4] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
};
//配置聚光灯(绿色调)
glm::vec3 g_spotlightColor = { 0.2f,1.0f,0.1f };

//设置鼠标初始位置(鼠标的起始位置设置带屏幕的中心)
float lastX = screenWidth / 2, lastY = screenHeight / 2;
bool firstMouse = true;//第一次移动鼠标

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


int main()
{
	glfwInit();	//初始化GLFW
	//配置GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//创建一个窗口于对象,这个窗口对象存放了和所有窗口相关的数据,而且会被频繁用到
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "ZJHOpenGL", NULL, NULL);
	if (window == nullptr) {
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();	//如果窗口创建失败,glfw停止工作,结束当前线程
		return -1;
	}
	glfwMakeContextCurrent(window);	//将此窗口的上下文设置成当前线程的主上下文

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//设置隐藏鼠标

	//GLAD是用来管理OpenGL的函数指针的,所以在调用任何OpenGL的函数之前我们需要初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	//视口
	//在我们开始渲染之前还有一件重要的事情要做的就是设置渲染窗口的大小,即视口的大小(viewport),这样OpenGL知道怎么根据窗口的大小显示数据和坐标(通常我们设置视口比窗口小一点,这样可以在视口外窗口内显示一些其他的可视元素)
	glViewport(0, 0, screenWidth, screenHeight);	//前两个参数是视口左下角的位置,后两个参数是视口的宽和高

	//当用户改变窗口的大小的时候视口大小也要相应的变化,我们注册一个回调函数,它会在每次窗口大小发生变化的时候被调用
	glfwSetFramebufferSizeCallback(window, Framebuffer_size_callback);	//注册这个函数,告诉GLFW每当窗口发小调整的时候调用这个函数(窗口第一次显示的时候这个函数也会被调用)
	glfwSetCursorPosCallback(window, Mouse_move_callback);//告诉GLFW当鼠标移动的时候调用这个函数
	glfwSetScrollCallback(window, Mouse_scroll_callback);//设置滚轮滚动回调函数

	//一个box模型
	float vertices[] = {
	-0.5f,-0.5f,0.5f , 0.0f,0.0f,1.0f , 0.0f,0.0f,
	0.5f,-0.5f,0.5f , 0.0f,0.0f,1.0f , 1.0f,0.0f,
	0.5f,0.5f,0.5f , 0.0f,0.0f,1.0f ,  1.0f,1.0f,
	-0.5f,0.5f,0.5f , 0.0f,0.0f,1.0f , 0.0f,1.0f,
	-0.5f,0.5f,-0.5f , 0.0f,0.0f,-1.0f , 0.0f,0.0f,
	0.5f,0.5f,-0.5f , 0.0f,0.0f,-1.0f , 1.0f,0.0f,
	0.5f,-0.5f,-0.5f , 0.0f,0.0f,-1.0f , 1.0f,1.0f,
	-0.5f,-0.5f,-0.5f , 0.0f,0.0f,-1.0f , 0.0f,1.0f,
	-0.5f,-0.5f,0.5f , -1.0f,0.0f,0 , 0.0f,0.0f,
	0.5f,-0.5f,0.5f , 1.0f,0.0f,0 , 0.0f,1.0f,
	-0.5f,0.5f,0.5f , -1.0f,0.0f,0 , 1.0f,0.0f,
	0.5f,0.5f,0.5f , 1.0f,0.0f,0 , 1.0f,1.0f,
	-0.5f,0.5f,-0.5f , -1.0f,0.0f,0 , 1.0f,1.0f,
	0.5f,0.5f,-0.5f , 1.0f,0.0f,0 , 1.0f,0.0f,
	-0.5f,-0.5f,-0.5f , -1.0f,0.0f,0 , 0.0f,1.0f,
	0.5f,-0.5f,-0.5f , 1.0f,0.0f,0 , 0.0f,0.0f,
	0.5f,0.5f,0.5f , 0.0f,1.0f,0 , 1.0f,0.0f,
	0.5f,0.5f,-0.5f , 0.0f,1.0f,0 , 1.0f,1.0f,
	-0.5f,0.5f,-0.5f , 0.0f,1.0f,0 , 0.0f,1.0f,
	-0.5f,0.5f,0.5f , 0.0f,1.0f,0 , 0.0f,0.0f,
	0.5f,-0.5f,-0.5f , 0.0f,-1.0f,0 , 0.0f,0.0f,
	0.5f,-0.5f,0.5f , 0.0f,-1.0f,0 , 1.0f,0.0f,
	-0.5f,-0.5f,0.5f , 0.0f,-1.0f,0 , 1.0f,1.0f,
	-0.5f,-0.5f,-0.5f , 0.0f,-1.0f,0 , 0.0f,1.0f
	};
	//顶点索引缓冲
	unsigned int indices[] = { 0,3,1,3,2,1,4,7,5,7,6,5,9,11,15,11,13,15,14,12,8,12,10,8,19,18,16,18,17,16,23,22,20,22,21,20 };
	glm::vec3 cubePositions[10] = {
				glm::vec3(0.5f,  0.0f,  0.0f),
				glm::vec3(2.0f,  5.0f, -15.0f),
				glm::vec3(-1.5f, -2.2f, -2.5f),
				glm::vec3(-3.8f, -2.0f, -12.3f),
				glm::vec3(2.4f, -0.4f, -3.5f),
				glm::vec3(-1.7f,  3.0f, -7.5f),
				glm::vec3(1.3f, -2.0f, -2.5f),
				glm::vec3(1.5f,  2.0f, -2.5f),
				glm::vec3(1.5f,  0.2f, -1.5f),
				glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	//灯泡模型
	float vertices_light[] = {
	-0.252599,-0.603628,0.103277,
	-0.259218,-0.542069,0.106019,
	-0.273484,-0.603628,-0.00171949,
	-0.280648,-0.542069,-0.00171949,
	-0.252598,-0.603628,-0.106716,
	-0.259218,-0.542069,-0.109458,
	-0.193122,-0.603628,-0.195728,
	-0.198189,-0.542069,-0.200794,
	-0.10411,-0.603628,-0.255204,
	-0.106852,-0.542069,-0.261823,
	0.00088588,-0.670457,-0.244558,
	0.0938166,-0.670457,-0.226073,
	0.172599,-0.670457,-0.173432,
	0.22524,-0.670457,-0.0946499,
	0.243725,-0.670457,-0.0017192,
	0.22524,-0.670457,0.0912112,
	0.172599,-0.670457,0.169994,
	0.0938166,-0.670457,0.222635,
	0.00088588,-0.670457,0.24112,
	-0.0920445,-0.670457,0.222635,
	-0.170827,-0.670457,0.169993,
	-0.223468,-0.670457,0.0912112,
	-0.241953,-0.670457,-0.00171949,
	-0.223468,-0.670457,-0.0946502,
	-0.170827,-0.670457,-0.173432,
	-0.0920442,-0.670457,-0.226073,
	0.00088588,-0.486209,-0.287913,
	0.110408,-0.486209,-0.266128,
	0.203255,-0.486209,-0.204089,
	0.265294,-0.486209,-0.111241,
	0.28708,-0.486209,-0.0017192,
	0.265295,-0.486209,0.107802,
	0.203255,-0.486209,0.20065,
	0.110408,-0.486209,0.262689,
	0.00088588,-0.486209,0.284475,
	-0.108636,-0.486209,0.262689,
	-0.201484,-0.486209,0.20065,
	-0.263523,-0.486209,0.107802,
	-0.285308,-0.486209,-0.00171949,
	-0.263522,-0.486209,-0.111241,
	-0.201483,-0.486209,-0.204089,
	-0.108635,-0.486209,-0.266128,
	0.00088588,-0.431745,-0.30348,
	0.116365,-0.431745,-0.28051,
	0.214263,-0.431745,-0.215096,
	0.279676,-0.431745,-0.117198,
	0.302647,-0.431745,-0.0017192,
	0.279676,-0.431745,0.113759,
	0.214263,-0.431745,0.211658,
	0.116365,-0.431745,0.277071,
	0.00088588,-0.431745,0.300041,
	-0.114593,-0.431745,0.277071,
	-0.212491,-0.431745,0.211658,
	-0.277905,-0.431745,0.113759,
	-0.300875,-0.431745,-0.00171949,
	-0.277904,-0.431745,-0.117198,
	-0.212491,-0.431745,-0.215097,
	-0.114592,-0.431745,-0.28051,
	0.00088588,-0.378678,-0.329779,
	0.126429,-0.378678,-0.304807,
	0.232859,-0.378678,-0.233692,
	0.303974,-0.378678,-0.127262,
	0.328946,-0.378678,-0.0017192,
	0.303974,-0.378678,0.123824,
	0.232859,-0.378678,0.230254,
	0.126429,-0.378678,0.301368,
	0.00088588,-0.378678,0.32634,
	-0.124657,-0.378678,0.301368,
	-0.231087,-0.378678,0.230254,
	-0.302202,-0.378678,0.123823,
	-0.327174,-0.378678,-0.00171949,
	-0.302201,-0.378678,-0.127262,
	-0.231087,-0.378678,-0.233693,
	-0.124657,-0.378678,-0.304807,
	0.00088588,-0.29768,-0.369036,
	0.141452,-0.29768,-0.341076,
	0.260618,-0.29768,-0.261451,
	0.340242,-0.29768,-0.142285,
	0.368203,-0.29768,-0.0017192,
	0.340242,-0.29768,0.138847,
	0.260618,-0.29768,0.258013,
	0.141452,-0.29768,0.337637,
	0.00088588,-0.29768,0.365598,
	-0.13968,-0.29768,0.337637,
	-0.258846,-0.29768,0.258013,
	-0.338471,-0.29768,0.138847,
	-0.366431,-0.29768,-0.00171949,
	-0.33847,-0.29768,-0.142286,
	-0.258846,-0.29768,-0.261452,
	-0.13968,-0.29768,-0.341076,
	0.00088588,-0.198528,-0.424666,
	0.162741,-0.198528,-0.392471,
	0.299954,-0.198528,-0.300788,
	0.391638,-0.198528,-0.163574,
	0.423833,-0.198528,-0.0017192,
	0.391638,-0.198528,0.160135,
	0.299954,-0.198528,0.297349,
	0.162741,-0.198528,0.389032,
	0.00088588,-0.198528,0.421227,
	-0.160969,-0.198528,0.389032,
	-0.298182,-0.198528,0.297349,
	-0.389866,-0.198528,0.160135,
	-0.42206,-0.198528,-0.00171949,
	-0.389865,-0.198528,-0.163574,
	-0.298182,-0.198528,-0.300788,
	-0.160968,-0.198528,-0.392471,
	0.00088588,-0.00581023,-0.499248,
	0.191282,-0.00581023,-0.461376,
	0.352692,-0.00581023,-0.353525,
	0.460543,-0.00581023,-0.192115,
	0.498415,-0.00581023,-0.0017192,
	0.460543,-0.00581023,0.188677,
	0.352692,-0.00581023,0.350087,
	0.191282,-0.00581023,0.457937,
	0.00088588,-0.00581023,0.49581,
	-0.18951,-0.00581023,0.457937,
	-0.35092,-0.00581023,0.350086,
	-0.458771,-0.00581023,0.188676,
	-0.496643,-0.00581023,-0.00171979,
	-0.45877,-0.00581023,-0.192116,
	-0.350919,-0.00581023,-0.353526,
	-0.189509,-0.00581023,-0.461376,
	0.00088588,0.140823,-0.507812,
	0.194559,0.140823,-0.469288,
	0.358748,0.140823,-0.359581,
	0.468455,0.140823,-0.195393,
	0.506979,0.140823,-0.0017192,
	0.468455,0.140823,0.191954,
	0.358748,0.140823,0.356142,
	0.194559,0.140823,0.46585,
	0.00088588,0.140823,0.504374,
	-0.192788,0.140823,0.46585,
	-0.356976,0.140823,0.356142,
	-0.466683,0.140823,0.191954,
	-0.505207,0.140823,-0.00171979,
	-0.466683,0.140823,-0.195393,
	-0.356975,0.140823,-0.359581,
	-0.192787,0.140823,-0.469288,
	0.00088588,0.260923,-0.473578,
	0.181459,0.260923,-0.43766,
	0.334541,0.260923,-0.335374,
	0.436827,0.260923,-0.182292,
	0.472745,0.260923,-0.0017192,
	0.436827,0.260923,0.178853,
	0.334541,0.260923,0.331935,
	0.181459,0.260923,0.434222,
	0.00088588,0.260923,0.47014,
	-0.179687,0.260923,0.434222,
	-0.332769,0.260923,0.331935,
	-0.435055,0.260923,0.178853,
	-0.470973,0.260923,-0.00171979,
	-0.435055,0.260923,-0.182292,
	-0.332768,0.260923,-0.335374,
	-0.179686,0.260923,-0.43766,
	0.00088588,0.389402,-0.398578,
	0.152757,0.389402,-0.368369,
	0.281507,0.389402,-0.282341,
	0.367536,0.389402,-0.15359,
	0.397745,0.389402,-0.0017192,
	0.367536,0.389402,0.150152,
	0.281507,0.389402,0.278902,
	0.152757,0.389402,0.36493,
	0.00088588,0.389402,0.39514,
	-0.150985,0.389402,0.36493,
	-0.279736,0.389402,0.278902,
	-0.365764,0.389402,0.150152,
	-0.395973,0.389402,-0.00171949,
	-0.365763,0.389402,-0.153591,
	-0.279735,0.389402,-0.282341,
	-0.150985,0.389402,-0.368369,
	0.00088588,0.513419,-0.268119,
	0.102833,0.513419,-0.247841,
	0.189259,0.513419,-0.190092,
	0.247008,0.513419,-0.103666,
	0.267286,0.513419,-0.0017192,
	0.247008,0.513419,0.100228,
	0.189259,0.513419,0.186654,
	0.102833,0.513419,0.244402,
	0.00088588,0.513419,0.264681,
	-0.101061,0.513419,0.244402,
	-0.187487,0.513419,0.186654,
	-0.245236,0.513419,0.100227,
	-0.265514,0.513419,-0.00171949,
	-0.245235,0.513419,-0.103666,
	-0.187487,0.513419,-0.190093,
	-0.10106,0.513419,-0.247841,
	0.00088588,0.579591,-0.0929356,
	0.0357929,0.579591,-0.0859923,
	0.0653858,0.579591,-0.0662191,
	0.0851589,0.579591,-0.0366262,
	0.0921023,0.579591,-0.0017192,
	0.0851589,0.579591,0.0331878,
	0.0653858,0.579591,0.0627804,
	0.0357929,0.579591,0.0825536,
	0.00088588,0.579591,0.0894969,
	-0.0340211,0.579591,0.0825536,
	-0.0636137,0.579591,0.0627804,
	-0.0833869,0.579591,0.0331875,
	-0.0903303,0.579591,-0.00171949,
	-0.0833869,0.579591,-0.0366265,
	-0.0636137,0.579591,-0.0662191,
	-0.0340208,0.579591,-0.0859923,
	0.00088588,0.588486,-0.00171949,
	0.00247807,-0.709282,-0.189536,
	0.0756773,-0.709282,-0.174358,
	0.137418,-0.709282,-0.132765,
	0.178301,-0.709282,-0.0710903,
	0.192101,-0.709282,0.00127743,
	0.176719,-0.709282,0.0733204,
	0.134495,-0.709282,0.134071,
	0.0718588,-0.709282,0.17428,
	-0.00165497,-0.709282,0.187827,
	-0.0748542,-0.709282,0.172648,
	-0.136595,-0.709282,0.131056,
	-0.177478,-0.709282,0.0693804,
	-0.191278,-0.709282,-0.002987,
	-0.175896,-0.709282,-0.07503,
	-0.133672,-0.709282,-0.135781,
	-0.0710357,-0.709282,-0.17599,
	0.00406997,-0.968293,-0.189259,
	0.078488,-0.968293,-0.173219,
	0.140947,-0.968293,-0.130808,
	0.181939,-0.968293,-0.0684802,
	0.195222,-0.968293,0.00427436,
	0.178775,-0.968293,0.0763798,
	0.135102,-0.968293,0.136859,
	0.0708511,-0.968293,0.176503,
	-0.0041961,-0.968293,0.189278,
	-0.0786141,-0.968293,0.173239,
	-0.141073,-0.968293,0.130828,
	-0.182065,-0.968293,0.0684998,
	-0.195348,-0.968293,-0.00425479,
	-0.178901,-0.968293,-0.07636,
	-0.135228,-0.968293,-0.136839,
	-0.0709767,-0.968293,-0.176483,
	0.00253121,-1.01158,-0.118777,
	0.0492364,-1.01158,-0.108711,
	0.088437,-1.01158,-0.0820931,
	0.114164,-1.01158,-0.0429751,
	0.122501,-1.01158,0.00268625,
	0.112178,-1.01158,0.0479403,
	0.0847685,-1.01158,0.085898,
	0.0444435,-1.01158,0.110779,
	-0.00265706,-1.01158,0.118797,
	-0.0493623,-1.01158,0.108731,
	-0.0885629,-1.01158,0.0821127,
	-0.11429,-1.01158,0.042995,
	-0.122627,-1.01158,-0.0026664,
	-0.112304,-1.01158,-0.0479207,
	-0.0848941,-1.01158,-0.0858781,
	-0.0445693,-1.01158,-0.11076,
	0.00203017,-1.03847,-0.0958371,
	0.039716,-1.03847,-0.0877147,
	0.0713455,-1.03847,-0.0662369,
	0.0921041,-1.03847,-0.034674,
	0.0988308,-1.03847,0.00216944,
	0.090502,-1.03847,0.0386841,
	0.0683856,-1.03847,0.0693109,
	0.0358484,-1.03847,0.0893875,
	-0.00215601,-1.03847,0.0958569,
	-0.0398418,-1.03847,0.0877345,
	-0.0714713,-1.03847,0.0662568,
	-0.0922299,-1.03847,0.0346939,
	-0.0989566,-1.03847,-0.00214959,
	-0.0906278,-1.03847,-0.0386643,
	-0.0685115,-1.03847,-0.0692911,
	-0.0359739,-1.03847,-0.0893676,
	-6.27766e-05,-1.04334,9.92746e-06
	};
	//灯泡的顶点索引缓冲
	unsigned int indices_light[] = { 0,2,1,2,3,1,2,4,3,4,5,3,4,6,5,6,7,5,6,8,7,8,9,7,8,10,9,10,11,9,10,12,11,12,13,11,12,14,13,14,15,13,14,16,15,16,17,15,16,18,17,18,19,17,18,20,19,20,21,19,20,22,21,22,23,21,22,24,23,24,25,23,24,26,25,26,27,25,26,28,27,28,29,27,28,30,29,30,31,29,1,48,31,48,63,31,31,30,1,30,0,1,28,46,30,46,47,30,62,61,29,61,27,29,25,60,23,60,59,23,21,58,19,58,57,19,17,56,15,56,55,15,9,52,7,52,51,7,13,54,11,54,53,11,5,50,3,50,49,3,31,63,29,63,62,29,27,61,25,61,60,25,23,59,21,59,58,21,19,57,17,57,56,17,11,53,9,53,52,9,3,49,1,49,48,1,15,55,13,55,54,13,7,51,5,51,50,5,24,44,26,44,45,26,20,42,22,42,43,22,16,40,18,40,41,18,0,32,2,32,33,2,12,38,14,38,39,14,4,34,6,34,35,6,8,36,10,36,37,10,30,47,0,47,32,0,26,45,28,45,46,28,22,43,24,43,44,24,18,41,20,41,42,20,10,37,12,37,38,12,2,33,4,33,34,4,14,39,16,39,40,16,6,35,8,35,36,8,46,239,47,239,240,47,44,237,45,237,238,45,42,235,43,235,236,43,40,233,41,233,234,41,32,225,33,225,226,33,38,231,39,231,232,39,36,229,37,229,230,37,34,227,35,227,228,35,47,240,32,240,225,32,45,238,46,238,239,46,43,236,44,236,237,44,41,234,42,234,235,42,37,230,38,230,231,38,35,228,36,228,229,36,33,226,34,226,227,34,39,232,40,232,233,40,63,79,62,79,78,62,61,77,60,77,76,60,59,75,58,75,74,58,57,73,56,73,72,56,55,71,54,71,70,54,53,69,52,69,68,52,51,67,50,67,66,50,49,65,48,65,64,48,48,64,63,64,79,63,78,77,62,77,61,62,60,76,59,76,75,59,58,74,57,74,73,57,50,66,49,66,65,49,56,72,55,72,71,55,54,70,53,70,69,53,52,68,51,68,67,51,78,79,94,79,95,94,94,95,110,95,111,110,77,93,76,93,92,76,75,91,74,91,90,74,73,89,72,89,88,72,67,83,66,83,82,66,65,81,64,81,80,64,71,87,70,87,86,70,69,85,68,85,84,68,64,80,79,80,95,79,78,94,77,94,93,77,76,92,75,92,91,75,74,90,73,90,89,73,72,88,71,88,87,71,70,86,69,86,85,69,68,84,67,84,83,67,66,82,65,82,81,65,111,127,110,127,126,110,93,109,92,109,108,92,91,107,90,107,106,90,89,105,88,105,104,88,81,97,80,97,96,80,87,103,86,103,102,86,85,101,84,101,100,84,83,99,82,99,98,82,80,96,95,96,111,95,94,110,93,110,109,93,92,108,91,108,107,91,90,106,89,106,105,89,86,102,85,102,101,85,84,100,83,100,99,83,82,98,81,98,97,81,88,104,87,104,103,87,109,125,108,125,124,108,107,123,106,123,122,106,105,121,104,121,120,104,103,119,102,119,118,102,101,117,100,117,116,100,99,115,98,115,114,98,97,113,96,113,112,96,96,112,111,112,127,111,126,125,110,125,109,110,108,124,107,124,123,107,106,122,105,122,121,105,100,116,99,116,115,99,98,114,97,114,113,97,104,120,103,120,119,103,102,118,101,118,117,101,127,143,126,143,142,126,125,141,124,141,140,124,123,139,122,139,138,122,121,137,120,137,136,120,117,133,116,133,132,116,115,131,114,131,130,114,113,129,112,129,128,112,119,135,118,135,134,118,112,128,127,128,143,127,142,141,126,141,125,126,124,140,123,140,139,123,122,138,121,138,137,121,120,136,119,136,135,119,118,134,117,134,133,117,116,132,115,132,131,115,114,130,113,130,129,113,143,159,142,159,158,142,143,128,159,128,144,159,141,157,140,157,156,140,139,155,138,155,154,138,137,153,136,153,152,136,135,151,134,151,150,134,133,149,132,149,148,132,131,147,130,147,146,130,129,145,128,145,144,128,142,158,141,158,157,141,140,156,139,156,155,139,138,154,137,154,153,137,132,148,131,148,147,131,130,146,129,146,145,129,136,152,135,152,151,135,134,150,133,150,149,133,160,175,144,175,159,144,158,174,157,174,173,157,156,172,155,172,171,155,154,170,153,170,169,153,152,168,151,168,167,151,150,166,149,166,165,149,148,164,147,164,163,147,146,162,145,162,161,145,159,175,158,175,174,158,157,173,156,173,172,156,155,171,154,171,170,154,153,169,152,169,168,152,145,161,144,161,160,144,151,167,150,167,166,150,149,165,148,165,164,148,147,163,146,163,162,146,176,191,160,191,175,160,174,190,173,190,189,173,172,188,171,188,187,171,170,186,169,186,185,169,164,180,163,180,179,163,162,178,161,178,177,161,168,184,167,184,183,167,166,182,165,182,181,165,175,191,174,191,190,174,173,189,172,189,188,172,171,187,170,187,186,170,169,185,168,185,184,168,167,183,166,183,182,166,165,181,164,181,180,164,163,179,162,179,178,162,161,177,160,177,176,160,192,207,176,207,191,176,190,206,189,206,205,189,188,204,187,204,203,187,186,202,185,202,201,185,184,200,183,200,199,183,182,198,181,198,197,181,180,196,179,196,195,179,178,194,177,194,193,177,191,207,190,207,206,190,189,205,188,205,204,188,187,203,186,203,202,186,185,201,184,201,200,184,179,195,178,195,194,178,177,193,176,193,192,176,183,199,182,199,198,182,181,197,180,197,196,180,208,223,192,223,207,192,206,222,205,222,221,205,204,220,203,220,219,203,202,218,201,218,217,201,198,214,197,214,213,197,196,212,195,212,211,195,194,210,193,210,209,193,200,216,199,216,215,199,207,223,206,223,222,206,205,221,204,221,220,204,203,219,202,219,218,202,201,217,200,217,216,200,199,215,198,215,214,198,197,213,196,213,212,196,195,211,194,211,210,194,193,209,192,209,208,192,224,223,208,222,224,221,220,224,219,218,224,217,210,224,209,216,224,215,214,224,213,212,224,211,223,224,222,221,224,220,219,224,218,217,224,216,213,224,212,211,224,210,209,224,208,215,224,214,225,241,226,241,242,226,226,242,227,242,243,227,227,243,228,243,244,228,228,244,229,244,245,229,229,245,230,245,246,230,230,246,231,246,247,231,231,247,232,247,248,232,232,248,233,248,249,233,233,249,234,249,250,234,234,250,235,250,251,235,235,251,236,251,252,236,236,252,237,252,253,237,237,253,238,253,254,238,254,255,238,255,239,238,255,256,239,256,240,239,240,256,225,256,241,225,255,271,256,271,272,256,271,287,272,287,288,272,253,269,254,269,270,254,267,268,251,268,252,251,265,266,249,266,250,249,259,260,243,260,244,243,245,261,246,261,262,246,247,263,248,263,264,248,256,272,241,272,257,241,254,270,255,270,271,255,253,252,269,252,268,269,251,250,267,250,266,267,246,262,247,262,263,247,243,242,259,242,258,259,248,264,249,264,265,249,245,244,261,244,260,261,242,241,258,241,257,258,269,285,270,285,286,270,268,267,284,267,283,284,266,265,282,265,281,282,263,279,264,279,280,264,261,277,262,277,278,262,260,259,276,259,275,276,258,257,274,257,273,274,272,288,257,288,273,257,270,286,271,286,287,271,269,268,285,268,284,285,267,266,283,266,282,283,264,280,265,280,281,265,262,278,263,278,279,263,261,260,277,260,276,277,259,258,275,258,274,275,288,287,289,285,289,286,283,289,284,281,289,282,273,289,274,279,289,280,277,289,278,275,289,276,289,273,288,289,287,286,289,285,284,289,283,282,289,277,276,289,275,274,289,281,280,289,279,278 };

	//创建一个顶点缓冲对象和顶点数组对象和一个元素缓冲对象(为光源创建单独的VBO和VAO)
	unsigned int EBO;
	unsigned int EBO_L;
	unsigned int VBO;
	unsigned int VBO_L;
	unsigned int VAO;
	unsigned int VAO_L;
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &EBO_L);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBO_L);
	glGenVertexArrays(1, &VAO);
	glGenVertexArrays(1, &VAO_L);

	//绑定箱子的VAO和VBO
	//绑定VAO
	glBindVertexArray(VAO);
	//绑定VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//绑定EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//填充数据
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	//解绑
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//绑定灯泡的VAO_L和VBO_L
	glBindVertexArray(VAO_L);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_L);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_L);
	//填充数据
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_light), indices_light, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_light), vertices_light, GL_STATIC_DRAW);
	//设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//解绑
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//创建顶点着色器
	Shader myShader1("shaders/openGLLearning16/vertexShader.vs.c", "shaders/openGLLearning16/fragmentShader1.fs.c");
	Shader myShader1_L("shaders/openGLLearning10/vertexShader.vs", "shaders/openGLLearning10/fragmentShader1.fs");
	//纹理
	//生成2个纹理对象
	unsigned int textures_diffuse, textures_specular;
	glGenTextures(1, &textures_diffuse);
	glGenTextures(1, &textures_specular);
	//绑定漫反射纹理
	glBindTexture(GL_TEXTURE_2D, textures_diffuse);
	//设置当前绑定的纹理对象的环绕和过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//读取纹理图片
	int tex_width, tex_height, tex_channelsNum;
	unsigned char* tex_data = stbi_load("assets/textures/mx_diffuse.png", &tex_width, &tex_height, &tex_channelsNum, 0);
	//使用glTexImage2D函数将图片数据生成纹理
	if (tex_data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
		glGenerateMipmap(GL_TEXTURE_2D);	//这会为当前绑定的纹理自动生成所需要的多级渐进纹理
	}
	else cout << "Failed to load texture1" << endl;
	//绑定高光反射纹理
	glBindTexture(GL_TEXTURE_2D, textures_specular);
	//设置当前绑定的纹理对象的环绕和过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//读取纹理图片
	tex_data = stbi_load("assets/textures/mx_specular.png", &tex_width, &tex_height, &tex_channelsNum, 0);
	//使用glTexImage2D函数将图片数据生成纹理
	if (tex_data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
		glGenerateMipmap(GL_TEXTURE_2D);	//这会为当前绑定的纹理自动生成所需要的多级渐进纹理
	}
	else cout << "Failed to load texture1" << endl;
	stbi_image_free(tex_data);	//生成纹理后释放data内存

	//设置采样器对应的纹理单元
	myShader1.Use();
	myShader1.SetInt("material.diffuse", 0);
	myShader1.SetInt("material.specular", 1);
	myShader1.SetVec3f("material.ambient", glm::vec3(1.0f, 0.5f, 0.31));
	myShader1.SetVec3f("material.specular", glm::vec3(0.6f, 0.5f, 0.4));
	myShader1.SetFloat("material.shininess", 64.0f);
	//配置平行光
	myShader1.SetVec3f("dirlight.ambient", g_dirlightColor * 0.1f);
	myShader1.SetVec3f("dirlight.diffuse", g_dirlightColor * 0.5f);
	myShader1.SetVec3f("dirlight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
	myShader1.SetVec3f("dirlight.direction", g_dirlightDirection);
	//配置点光源
	myShader1.SetVec3f("pointlights[0].position", g_pointlightPositions[0]);
	myShader1.SetVec3f("pointlights[1].position", g_pointlightPositions[1]);
	myShader1.SetVec3f("pointlights[2].position", g_pointlightPositions[2]);
	myShader1.SetVec3f("pointlights[3].position", g_pointlightPositions[3]);
	myShader1.SetVec3f("pointlights[0].ambient", g_pointlightColor * 0.1f);
	myShader1.SetVec3f("pointlights[1].ambient", g_pointlightColor * 0.1f);
	myShader1.SetVec3f("pointlights[2].ambient", g_pointlightColor * 0.1f);
	myShader1.SetVec3f("pointlights[3].ambient", g_pointlightColor * 0.1f);
	myShader1.SetVec3f("pointlights[0].diffuse", g_pointlightColor * 0.6f);
	myShader1.SetVec3f("pointlights[1].diffuse", g_pointlightColor * 0.6f);
	myShader1.SetVec3f("pointlights[2].diffuse", g_pointlightColor * 0.6f);
	myShader1.SetVec3f("pointlights[3].diffuse", g_pointlightColor * 0.6f);
	myShader1.SetVec3f("pointlights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	myShader1.SetVec3f("pointlights[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	myShader1.SetVec3f("pointlights[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	myShader1.SetVec3f("pointlights[3].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	myShader1.SetFloat("pointlights[0].kc", 1.0f);
	myShader1.SetFloat("pointlights[1].kc", 1.0f);
	myShader1.SetFloat("pointlights[2].kc", 1.0f);
	myShader1.SetFloat("pointlights[3].kc", 1.0f);
	myShader1.SetFloat("pointlights[0].kl", 0.09f);
	myShader1.SetFloat("pointlights[1].kl", 0.09f);
	myShader1.SetFloat("pointlights[2].kl", 0.09f);
	myShader1.SetFloat("pointlights[3].kl", 0.09f);
	myShader1.SetFloat("pointlights[0].kq", 0.032f);
	myShader1.SetFloat("pointlights[1].kq", 0.032f);
	myShader1.SetFloat("pointlights[2].kq", 0.032f);
	myShader1.SetFloat("pointlights[3].kq", 0.032f);

	//配置聚光灯
	myShader1.SetVec3f("spotlight.ambient", g_spotlightColor * 0.1f);
	myShader1.SetVec3f("spotlight.diffuse", g_spotlightColor * 0.6f);
	myShader1.SetVec3f("spotlight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
	myShader1.SetFloat("spotlight.cutoff", glm::cos(glm::radians(12.5f)));
	myShader1.SetFloat("spotlight.outercutoff", glm::cos(glm::radians(17.5f)));

	myShader1_L.Use();
	myShader1_L.SetVec3f("lightColor", g_pointlightColor);
	//开启深度测试
	glEnable(GL_DEPTH_TEST);

	//保持渲染循环
	while (!glfwWindowShouldClose(window)) {	//如果检测到GLFW要求被关闭就结束循环
		ProcessInput(window, myShader1);	//按键响应
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		zcamera.cameraMoveSpeed = 2.50f * deltaTime;

		//渲染指令
		glClearColor(0.1f, 0.21f, 0.2f, 1.0f);	//设置颜色缓冲区的颜色值
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//清除颜色缓冲和深度缓冲

		//激活这个程序对象
		myShader1.Use();
		glActiveTexture(GL_TEXTURE0);//激活第0号采样器(默认被激活)
		glBindTexture(GL_TEXTURE_2D, textures_diffuse);//绑定第一个textures_diffuse到第0号采样器
		glActiveTexture(GL_TEXTURE1);//激活第1号采样器
		glBindTexture(GL_TEXTURE_2D, textures_specular);//绑定第一个textures_specular到第1号采样器
		glBindVertexArray(VAO);//每次绘制都绑定一次VAO
		myShader1.SetVec3f("viewPos", zcamera.cameraPos);
		//以相机的位置和方向设置聚光灯
		myShader1.SetVec3f("spotlight.position", zcamera.cameraPos);
		myShader1.SetVec3f("spotlight.direction", zcamera.GetCameraFront());
		glUniformMatrix4fv(glGetUniformLocation(myShader1.ID, "v_matrix"), 1, GL_FALSE, glm::value_ptr(zcamera.GetCameraViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(myShader1.ID, "p_matrix"), 1, GL_FALSE, glm::value_ptr(zcamera.GetCameraperspectiveMatrix()));
		//绘制被照射的箱子10个
		for (unsigned int i = 0; i < 10; i++) {
			glm::mat4 tempModel = glm::mat4(1.0f);
			tempModel = glm::translate(tempModel, cubePositions[i]);
			tempModel = glm::rotate(tempModel, (float)glm::radians(i * 20.0f), glm::vec3(1.0f, 0.5f, 0.0f));
			glUniformMatrix4fv(glGetUniformLocation(myShader1.ID, "m_matrix"), 1, GL_FALSE, glm::value_ptr(tempModel));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		}
		glBindVertexArray(0);//解绑

		//绘制4个点光源
		myShader1_L.Use();
		glUniformMatrix4fv(glGetUniformLocation(myShader1_L.ID, "v_matrix"), 1, GL_FALSE, glm::value_ptr(zcamera.GetCameraViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(myShader1_L.ID, "p_matrix"), 1, GL_FALSE, glm::value_ptr(zcamera.GetCameraperspectiveMatrix()));
		glBindVertexArray(VAO_L);
		for (unsigned int i = 0; i < 4; i++) {
			glm::mat4 tempModel_ = glm::mat4(1.0f);
			tempModel_ = glm::translate(tempModel_, g_pointlightPositions[i]);
			tempModel_ = glm::scale(tempModel_, glm::vec3(0.2));
			glUniformMatrix4fv(glGetUniformLocation(myShader1_L.ID, "m_matrix"), 1, GL_FALSE, glm::value_ptr(tempModel_));
			glDrawElements(GL_TRIANGLES, 1728, GL_UNSIGNED_INT, 0);
		}
		glBindVertexArray(0);//解绑

		glfwSwapBuffers(window);	//此函数会交换颜色缓冲(它是存储着GLFW窗口每一个像素颜色值的大缓冲),它在这一迭代中被用来绘制,并且会作为输出显示在屏幕上
		glfwPollEvents();	//此函数检查有没有鼠标键盘窗口等触发事件,如果有并调用相应的回调函数
	}

	//结束之前释放VAO VBO和EBO资源
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO_L);
	glDeleteBuffers(1, &EBO_L);
	glDeleteVertexArrays(1, &VAO_L);

	glfwTerminate();	//渲染结束后我们需要释放所有的分配资源,此函数可以完成
	return 0;
}

