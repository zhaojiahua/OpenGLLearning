//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
//
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//
//#include <iostream>
//#include <filesystem>
//
//#include "Shader.h"
//#include "ZCamera.h"
//#include "ZDirectionLight.h"
//#include "ZPointLight.h"
//#include "ZSpotLight.h"
//#include "ZMesh.h"
//
//using namespace std;
//
////设置屏幕宽高
//unsigned int screenWidth = 1400;
//unsigned int screenHeight = 1000;
////计算deltaTime
//float deltaTime = 0.0f;
//float lastFrame = 0.0f;
//
////创建一个摄像机
//ZCamera zcamera(45.0f, static_cast<float>(screenWidth) / static_cast<float>(screenHeight), 0.1f, 500.0f);
////配置平行光源(红色调)
//ZDirectionLight directionLight(glm::vec4(1.0f, 0.3f, 0.2f, 1.0f), glm::vec3(-0.2f, -1.0f, -0.3f));
////配置聚光灯(绿色调)
//ZSpotLight spotLight(glm::vec4(0.2f, 1.0f, 0.1f, 1.0f), glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(17.5f)));
//
////设置鼠标初始位置(鼠标的起始位置设置带屏幕的中心)
//float lastX = screenWidth / 2, lastY = screenHeight / 2;
//bool firstMouse = true;//第一次移动鼠标
//
////窗口大小发生变化时候的回调函数
//void Framebuffer_size_callback(GLFWwindow* inWind, int inWidth, int inHeight) {
//	glViewport(0, 20, inWidth, inHeight - 40);
//}
////鼠标移动回调函数
//void Mouse_move_callback(GLFWwindow* inWind, double xpos, double ypos) {
//	if (firstMouse) {
//		lastX = xpos;
//		lastY = ypos;
//		firstMouse = false;
//	}
//	float xoffset = (xpos - lastX) * zcamera.sensitivity;
//	float yoffset = (ypos - lastY) * zcamera.sensitivity;
//	lastX = xpos;
//	lastY = ypos;
//	zcamera.SetCameraFront(xoffset, yoffset);
//}
////鼠标滚轮回调函数
//void Mouse_scroll_callback(GLFWwindow* inWind, double xoffset, double yoffset) {
//	zcamera.SetCameraFov(yoffset);
//}
//
////输入响应函数
//void ProcessInput(GLFWwindow* inWind, Shader inshader) {
//	if (glfwGetKey(inWind, GLFW_KEY_ESCAPE) == GLFW_PRESS) {//如果检测到ESC键被按下
//		glfwSetWindowShouldClose(inWind, true);	//设置glfwWindowShouldClose为TRUE
//	}
//	if (glfwGetKey(inWind, GLFW_KEY_UP) == GLFW_PRESS) {//如果检测到up键被按下
//		inshader.Use();
//		float tempvalue = inshader.GetFloat("mixValue");
//		tempvalue += 0.0002;
//		if (tempvalue > 1.0f) tempvalue = 1.0f;
//		inshader.SetFloat("mixValue", tempvalue);
//	}
//	if (glfwGetKey(inWind, GLFW_KEY_DOWN) == GLFW_PRESS) {//如果检测到down键被按下
//		inshader.Use();
//		float tempvalue = inshader.GetFloat("mixValue");
//		tempvalue -= 0.0002;
//		if (tempvalue < 0.0f) tempvalue = 0.0f;
//		inshader.SetFloat("mixValue", tempvalue);
//	}
//	if (glfwGetKey(inWind, GLFW_KEY_W) == GLFW_PRESS) {//W键按下相机向前运动
//		zcamera.MoveForward();
//	}
//	if (glfwGetKey(inWind, GLFW_KEY_S) == GLFW_PRESS) {//S键按下相机向后运动
//		zcamera.MoveBackward();
//	}
//	if (glfwGetKey(inWind, GLFW_KEY_A) == GLFW_PRESS) {//A键按下相机向左运动
//		zcamera.MoveLeft();
//	}
//	if (glfwGetKey(inWind, GLFW_KEY_D) == GLFW_PRESS) {//D键按下相机向右运动
//		zcamera.MoveRight();
//	}
//	if (glfwGetKey(inWind, GLFW_KEY_SPACE) == GLFW_PRESS) {//空格键按下相机向上运动
//		zcamera.MoveUpWard();
//	}
//	if (glfwGetKey(inWind, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {//shift+空格键按下相机向下运动
//		zcamera.MoveDownWard();
//	}
//}
//
////加载天空盒子
//unsigned int LoadCubeMap(vector<string> textures_faces) {
//	unsigned int texture_box;
//	glGenTextures(1, &texture_box);
//	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_box);
//	//遍历纹理目标
//	int texC_width, texC_height, texC_nrChannels;
//	for (unsigned int i = 0; i < textures_faces.size(); i++) {
//		unsigned char* tempdata = stbi_load(textures_faces[i].c_str(), &texC_width, &texC_height, &texC_nrChannels, 0);
//		if (tempdata) {
//			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, texC_width, texC_height, 0, GL_RGB, GL_UNSIGNED_BYTE, tempdata);
//		}
//		else cout << "Cubemap texture failed to load at path:" << textures_faces[i] << endl;
//		stbi_image_free(tempdata);
//	}
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
//	return texture_box;
//}
//
//int main()
//{
//	glfwInit();	//初始化GLFW
//	//配置GLFW
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//	//创建一个窗口于对象,这个窗口对象存放了和所有窗口相关的数据,而且会被频繁用到
//	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "ZJHOpenGL", NULL, NULL);
//	if (window == nullptr) {
//		cout << "Failed to create GLFW window" << endl;
//		glfwTerminate();	//如果窗口创建失败,glfw停止工作,结束当前线程
//		return -1;
//	}
//	glfwMakeContextCurrent(window);	//将此窗口的上下文设置成当前线程的主上下文
//
//	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//设置隐藏鼠标
//
//	//GLAD是用来管理OpenGL的函数指针的,所以在调用任何OpenGL的函数之前我们需要初始化GLAD
//	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
//		cout << "Failed to initialize GLAD" << endl;
//		return -1;
//	}
//	//视口
//	//在我们开始渲染之前还有一件重要的事情要做的就是设置渲染窗口的大小,即视口的大小(viewport),这样OpenGL知道怎么根据窗口的大小显示数据和坐标(通常我们设置视口比窗口小一点,这样可以在视口外窗口内显示一些其他的可视元素)
//	glViewport(0, 0, screenWidth, screenHeight);	//前两个参数是视口左下角的位置,后两个参数是视口的宽和高
//
//	//当用户改变窗口的大小的时候视口大小也要相应的变化,我们注册一个回调函数,它会在每次窗口大小发生变化的时候被调用
//	glfwSetFramebufferSizeCallback(window, Framebuffer_size_callback);	//注册这个函数,告诉GLFW每当窗口发小调整的时候调用这个函数(窗口第一次显示的时候这个函数也会被调用)
//	glfwSetCursorPosCallback(window, Mouse_move_callback);//告诉GLFW当鼠标移动的时候调用这个函数
//	glfwSetScrollCallback(window, Mouse_scroll_callback);//设置滚轮滚动回调函数
//
//	//加载模型数据
//	ZModel testModel("assets/models/nanosuit/nanosuit.fbx");
//	//ZModel testModel("assets/models/box.fbx");
//	//ZModel testModel("assets/models/lowpolyscene/LowPolyWinterScene.obj");
//	//ZModel testModel("assets/models/small-scene/smallscene.fbx");
//	//ZModel testModel("assets/models/boxes/boxes.fbx");
//	//ZModel testModel_Ground("assets/models/ground/ground.obj");
//	//testModel.meshes[3].PrintMM();
//
//	float simplepoints[] = {
//		-0.5f,	0.5f,		0.0f,			1.0f,	0.2f,	0.0f,
//		 0.5f,		0.5f,		0.0f,			0.0f,	1.0f,	0.2f,
//		 0.5f,		-0.5f,	0.0f,			1.0f,	0.9f,	0.1f,
//		-0.5f,	-0.5f,	0.0f,			0.0f,	0.2f,	1.0f
//	};
//	unsigned int simpleVAO, simpleVBO;
//	glGenBuffers(1, &simpleVBO);
//	glGenVertexArrays(1, &simpleVAO);
//	glBindVertexArray(simpleVAO);
//	glBindBuffer(GL_ARRAY_BUFFER, simpleVBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(simplepoints), simplepoints, GL_STATIC_DRAW);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
//	glEnableVertexAttribArray(1);
//	glBindVertexArray(0);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//	//天空盒模型数据
//	float skyboxVertices[] = {
//		-1.0f,  1.0f, -1.0f,
//		-1.0f, -1.0f, -1.0f,
//		 1.0f, -1.0f, -1.0f,
//		 1.0f, -1.0f, -1.0f,
//		 1.0f,  1.0f, -1.0f,
//		-1.0f,  1.0f, -1.0f,
//
//		-1.0f, -1.0f,  1.0f,
//		-1.0f, -1.0f, -1.0f,
//		-1.0f,  1.0f, -1.0f,
//		-1.0f,  1.0f, -1.0f,
//		-1.0f,  1.0f,  1.0f,
//		-1.0f, -1.0f,  1.0f,
//
//		 1.0f, -1.0f, -1.0f,
//		 1.0f, -1.0f,  1.0f,
//		 1.0f,  1.0f,  1.0f,
//		 1.0f,  1.0f,  1.0f,
//		 1.0f,  1.0f, -1.0f,
//		 1.0f, -1.0f, -1.0f,
//
//		-1.0f, -1.0f,  1.0f,
//		-1.0f,  1.0f,  1.0f,
//		 1.0f,  1.0f,  1.0f,
//		 1.0f,  1.0f,  1.0f,
//		 1.0f, -1.0f,  1.0f,
//		-1.0f, -1.0f,  1.0f,
//
//		-1.0f,  1.0f, -1.0f,
//		 1.0f,  1.0f, -1.0f,
//		 1.0f,  1.0f,  1.0f,
//		 1.0f,  1.0f,  1.0f,
//		-1.0f,  1.0f,  1.0f,
//		-1.0f,  1.0f, -1.0f,
//
//		-1.0f, -1.0f, -1.0f,
//		-1.0f, -1.0f,  1.0f,
//		 1.0f, -1.0f, -1.0f,
//		 1.0f, -1.0f, -1.0f,
//		-1.0f, -1.0f,  1.0f,
//		 1.0f, -1.0f,  1.0f
//	};
//	unsigned int boxVAO, boxVBO;
//	glGenBuffers(1, &boxVBO);
//	glGenVertexArrays(1, &boxVAO);
//	glBindVertexArray(boxVAO);
//	glBindBuffer(GL_ARRAY_BUFFER, boxVBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//	glBindVertexArray(0);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//	//创建立方体贴图
//	vector<string>faces{//贴图路径
//		"assets/textures/skybox/right.jpg",
//		"assets/textures/skybox/left.jpg",
//		"assets/textures/skybox/top.jpg",
//		"assets/textures/skybox/bottom.jpg",
//		"assets/textures/skybox/front.jpg",
//		"assets/textures/skybox/back.jpg",
//	};
//	unsigned int cubemap = LoadCubeMap(faces);
//
//
//	//创建着色器
//	/*Shader myShader1;
//	myShader1.GenVertexShader("shaders/openGLLearning25/vertexShader.vs.c");
//	myShader1.GenGeometryShader("shaders/openGLLearning25/geometryShader.vs.c");
//	myShader1.GenFragmentShader("shaders/openGLLearning25/fragmentShader1.fs.c");*/
//	//Shader myShader1("shaders/openGLLearning25/vertexShader.vs.c", "shaders/openGLLearning25/fragmentShader1.fs.c");
//	//Shader myShader1_geometry("shaders/openGLLearning26/vertexShader.vs.c", "shaders/openGLLearning26/fragmentShader1.fs.c");
//	//Shader myShader1("shaders/openGLLearning26/vertexShader.vs.c", "shaders/openGLLearning26/fragmentShader1.fs.c");//不带几何着色器
//	Shader myShader1("shaders/openGLLearning27/vertexShader.vs.c", "shaders/openGLLearning27/fragmentShader1.fs.c", "shaders/openGLLearning27/geometryShader.vs.c");//带几何着色器
//	Shader myShader1_skybox("shaders/openGLLearning23/vertexShader_skybox.vs.c", "shaders/openGLLearning23/fragmentShader1_skybox.fs.c");
//
//	//设置采样器对应的纹理单元
//	myShader1.Use();
//	myShader1.SetFloat("material.shininess", 64.0f);
//	//配置平行光
//	directionLight.SetLight(&myShader1, 0, 0.1f, 0.5f, glm::vec4(1.0f));
//	//配置聚光灯
//	spotLight.SetLight(&myShader1, 0, 0.1f, 0.3f, glm::vec4(1.0f));
//
//	myShader1_skybox.Use();
//	myShader1_skybox.SetInt("cubemap", 0);
//
//	//保持渲染循环
//	while (!glfwWindowShouldClose(window)) {	//如果检测到GLFW要求被关闭就结束循环
//		ProcessInput(window, myShader1);	//按键响应
//		float currentFrame = glfwGetTime();
//		deltaTime = currentFrame - lastFrame;
//		lastFrame = currentFrame;
//		zcamera.cameraMoveSpeed = 10.0f * deltaTime;
//
//		//渲染指令
//		glClearColor(0.1f, 0.21f, 0.2f, 1.0f);	//设置颜色缓冲区的颜色值
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//清除颜色缓冲和深度缓冲
//
//		//第一个渲染天空盒子并且将深度写入关闭,这样盒子永远绘制在其他物体的背后
//		glDepthFunc(GL_LEQUAL);
//		myShader1_skybox.Use();
//		glm::mat4 viewM = glm::mat4(glm::mat3(zcamera.GetCameraViewMatrix()));//去除相机的位移信息,只保留相机的旋转信息
//		glUniformMatrix4fv(glGetUniformLocation(myShader1_skybox.ID, "v_matrix"), 1, GL_FALSE, glm::value_ptr(viewM));
//		glUniformMatrix4fv(glGetUniformLocation(myShader1_skybox.ID, "p_matrix"), 1, GL_FALSE, glm::value_ptr(zcamera.GetCameraperspectiveMatrix()));
//		glBindVertexArray(boxVAO);
//		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
//		glDrawArrays(GL_TRIANGLES, 0, 36);
//		glDepthFunc(GL_LESS);
//		//然后绘制其他场景
//		//激活这个程序对象
//		myShader1.Use();
//		myShader1.SetVec3f("viewPos", zcamera.cameraPos);
//		glUniformMatrix4fv(glGetUniformLocation(myShader1.ID, "v_matrix"), 1, GL_FALSE, glm::value_ptr(zcamera.GetCameraViewMatrix()));
//		glUniformMatrix4fv(glGetUniformLocation(myShader1.ID, "p_matrix"), 1, GL_FALSE, glm::value_ptr(zcamera.GetCameraperspectiveMatrix()));
//		glm::mat4 tempModelM = glm::mat4(1.0f);
//		tempModelM = glm::scale(tempModelM, glm::vec3(0.8f));
//		myShader1.SetFloat("time",glfwGetTime());
//		testModel.Draw(&myShader1, glm::mat4(1.0f), zcamera.cameraPos, GL_TRIANGLES);
//
//		glfwSwapBuffers(window);	//此函数会交换颜色缓冲(它是存储着GLFW窗口每一个像素颜色值的大缓冲),它在这一迭代中被用来绘制,并且会作为输出显示在屏幕上
//		glfwPollEvents();	//此函数检查有没有鼠标键盘窗口等触发事件,如果有并调用相应的回调函数
//	}
//	//释放缓冲内存
//	glDeleteVertexArrays(1, &boxVAO);
//	glDeleteBuffers(1, &boxVBO);
//	glDeleteVertexArrays(1, &simpleVAO);
//	glDeleteBuffers(1, &simpleVBO);
//
//	glfwTerminate();	//渲染结束后我们需要释放所有的分配资源,此函数可以完成
//	return 0;
//}
//
