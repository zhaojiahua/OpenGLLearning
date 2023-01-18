//#include "GlobalFunctions.h"
//
//int main()
//{
//	glfwInit();	//初始化GLFW
//	//配置GLFW
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//	glfwWindowHint(GLFW_SAMPLES, 4);	//创建多重采样样本缓冲(每个像素存储4个子样本)
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
//	//ZModel testModel("assets/models/nanosuit/nanosuit.fbx");
//	//ZModel testModel("assets/models/box.fbx");
//	//ZModel testModel("assets/models/lowpolyscene/LowPolyWinterScene.obj");
//	//ZModel testModel("assets/models/woodfloor/floor.fbx");
//	ZModel testModel("assets/models/woodfloor/pointlightScene2.fbx");
//	ZModel testModel_square("assets/models/woodfloor/square.obj");
//	//ZModel testModel_showMap("assets/models/woodfloor/showshadowmap.obj");
//	//ZModel testModel("assets/models/boxes/boxes.fbx");
//	//ZModel testModel("assets/models/ground/ground.obj");
//	//ZModel testModel("assets/models/skybox/box.fbx");
//	//ZModel testModel("assets/models/aixia/aixia.obj");
//	ZModel testModel_Light("assets/models/lightsMod/pointL.obj");
//	//cout << testModel.meshes[6].mTextures.size() << endl;
//	//cout << testModel.meshes.size() << endl;
//
//	//配置点光源(蓝色调)(为了便于观察效果我们把点光源衰减程度降低)
//	ZPointLight pointLight(glm::vec4(200.0f, 199.0f, 195.0f, 1.0f), glm::vec3(0.0f, 15.0f, -1.0f), 0.8f, 0.02f, 0.001f);
//	ZPointLight pointLight2(glm::vec4(0.8f, 0.7f, 0.55f, 1.0f), glm::vec3(0.0f, 10.0f, 220.0f), 0.3f, 0.01f, 0.0f);
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
//	//创建天空盒子立方体贴图
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
//	//创建着色器
//	Shader myShader1_light("shaders/openGLLearning37/vertexShader_forlight.vs.c", "shaders/openGLLearning37/fragmentShader1_forlight.fs.c");//渲染光源用的
//	Shader myShader1_skybox("shaders/openGLLearning23/vertexShader_skybox.vs.c", "shaders/openGLLearning23/fragmentShader1_skybox.fs.c");//渲染天空盒子用的
//	//Shader myShader1_square("shaders/openGLLearning39/vertexShader_square.vs.c", "shaders/openGLLearning39/fragmentShader1_square.fs.c");//渲染面片用的
//	//Shader myShader1_square("shaders/openGLLearning39/vertexShader_square.vs.c", "shaders/openGLLearning39/fragmentShader1_square_ReinhardToneMapping.fs.c");
//	Shader myShader1_square("shaders/openGLLearning39/vertexShader_square.vs.c", "shaders/openGLLearning39/fragmentShader1_square_ExposureToneMapping.fs.c");
//	Shader myShader1("shaders/openGLLearning39/vertexShader.vs.c", "shaders/openGLLearning39/fragmentShader1.fs.c");
//
//	//设置采样器对应的纹理单元
//	myShader1.Use();
//	myShader1.SetFloat("material.shininess", 64.0f);
//	//把灯光信息传递给shader
//	glm::vec4 specularColor = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);//高光颜色
//	pointLight.SetLight(&myShader1, 0, 0.1f, 0.4f, specularColor);
//	pointLight2.SetLight(&myShader1, 1, 0.1f, 0.4f, specularColor);
//
//	myShader1_skybox.Use();
//	myShader1_skybox.SetInt("cubemap", 0);
//
//	glm::mat4 lightMM(1.0f);
//	lightMM = glm::translate(lightMM, pointLight.mLightPosition);
//	myShader1_light.Use();
//	myShader1_light.SetVec4f("lightColor", pointLight.mLightColor);
//	glm::mat4 lightMM2(1.0f);
//	lightMM2 = glm::translate(lightMM2, pointLight2.mLightPosition);
//	myShader1_light.SetVec4f("lightColor", pointLight2.mLightColor);
//
//	glm::mat4 squareMM = glm::mat4(1.0f);
//	squareMM = glm::scale(squareMM, glm::vec3(2.0f));
//	myShader1_square.Use();
//	//高曝光会是使暗部细节显示更多而使亮部更亮;低曝光会使亮部细节显示更多而使暗部更暗;
//	//myShader1_square.SetFloat("exposure",5.0f);
//	myShader1_square.SetFloat("exposure",0.1f);
//
//	//生成一个浮点帧缓冲来存储渲染的HDR图像
//	unsigned int hdrFBO;
//	glGenFramebuffers(1, &hdrFBO);
//	//生成一张纹理附加颜色缓冲,存放颜色缓冲数据
//	unsigned int hdrColorMap;
//	glGenTextures(1, &hdrColorMap);
//	glBindTexture(GL_TEXTURE_2D, hdrColorMap);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);//颜色缓冲内部格式使用浮点类型
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	unsigned int rboDepth;
//	glGenRenderbuffers(1, &rboDepth);
//	glBindRenderbuffer(GL_RENDERBUFFER,rboDepth);
//	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screenWidth, screenHeight);
//	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, hdrColorMap, 0);
//	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
//	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)cout << "framebuffer not complete" << endl;
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	glClearColor(0.1f, 0.21f, 0.2f, 1.0f);	//设置颜色缓冲区的颜色值
//
//	//保持渲染循环
//	while (!glfwWindowShouldClose(window)) {	//如果检测到GLFW要求被关闭就结束循环
//		ProcessInput(window, myShader1);	//按键响应
//		float currentFrame = glfwGetTime();
//		deltaTime = currentFrame - lastFrame;
//		lastFrame = currentFrame;
//		zcamera.cameraMoveSpeed = 20.0f * deltaTime;
//
//		//渲染指令
//		
//		//渲染场景到浮点帧缓冲的颜色缓冲中
//		glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);//指定渲染缓冲区
//		//glBindFramebuffer(GL_FRAMEBUFFER, 0);//指定渲染缓冲区
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//清除默认帧缓冲区的颜色缓冲和深度缓冲
//		glViewport(0, 0, screenWidth, screenHeight);//记得把视口分辨率设置回来
//		//开启深度测试
//		glEnable(GL_DEPTH_TEST);
//		glEnable(GL_MULTISAMPLE);//开启多重采样绘制(默认都是开启的)
//		glEnable(GL_FRAMEBUFFER_SRGB);//开启gamma校正
//		//渲染场景物体
//		myShader1.Use();
//		myShader1.SetVec3f("viewPos", zcamera.cameraPos);
//		myShader1.SetMat4f("v_matrix", zcamera.GetCameraViewMatrix());
//		myShader1.SetMat4f("p_matrix", zcamera.GetCameraperspectiveMatrix());
//		testModel.Draw(&myShader1, glm::mat4(1.0f), zcamera.cameraPos, GL_TRIANGLES);
//		//渲染点光物体
//		myShader1_light.Use();
//		myShader1_light.SetVec3f("viewPos", zcamera.cameraPos);
//		myShader1_light.SetMat4f("v_matrix", zcamera.GetCameraViewMatrix());
//		myShader1_light.SetMat4f("p_matrix", zcamera.GetCameraperspectiveMatrix());
//		testModel_Light.Draw(&myShader1_light, lightMM, zcamera.cameraPos, GL_TRIANGLES);
//		testModel_Light.Draw(&myShader1_light, lightMM2, zcamera.cameraPos, GL_TRIANGLES);
//		glBindFramebuffer(GL_FRAMEBUFFER, 0);//场景渲染完成解绑浮点缓冲区
//
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//清除默认帧缓冲区的颜色缓冲和深度缓冲
//		//使用一个方片把渲染的帧缓冲中的颜色纹理显示出来
//		myShader1_square.Use();
//		myShader1_square.SetMat4f("v_matrix", zcamera.GetCameraViewMatrix());
//		myShader1_square.SetMat4f("p_matrix", zcamera.GetCameraperspectiveMatrix());
//		//给方片设置diffuse贴图
//		testModel_square.meshes[0].AddHeightMap(hdrColorMap);
//		testModel_square.Draw(&myShader1_square, squareMM, zcamera.cameraPos, GL_TRIANGLES);
//
//		//渲染天空盒子并且将深度写入关闭,这样盒子永远绘制在其他物体的背后
//		glDepthFunc(GL_LEQUAL);
//		myShader1_skybox.Use();
//		glm::mat4 viewM = glm::mat4(glm::mat3(zcamera.GetCameraViewMatrix()));//去除相机的位移信息,只保留相机的旋转信息
//		glUniformMatrix4fv(glGetUniformLocation(myShader1_skybox.ID, "v_matrix"), 1, GL_FALSE, glm::value_ptr(viewM));
//		glUniformMatrix4fv(glGetUniformLocation(myShader1_skybox.ID, "p_matrix"), 1, GL_FALSE, glm::value_ptr(zcamera.GetCameraperspectiveMatrix()));
//		glBindVertexArray(boxVAO);
//		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
//		glDrawArrays(GL_TRIANGLES, 0, 36);
//		glDepthFunc(GL_LESS);
//
//		glfwSwapBuffers(window);	//此函数会交换颜色缓冲(它是存储着GLFW窗口每一个像素颜色值的大缓冲),它在这一迭代中被用来绘制,并且会作为输出显示在屏幕上
//		glfwPollEvents();	//此函数检查有没有鼠标键盘窗口等触发事件,如果有并调用相应的回调函数
//	}
//	//释放缓冲内存
//	glDeleteVertexArrays(1, &boxVAO);
//	glDeleteBuffers(1, &boxVBO);
//
//	glfwTerminate();	//渲染结束后我们需要释放所有的分配资源,此函数可以完成
//	return 0;
//}
//
