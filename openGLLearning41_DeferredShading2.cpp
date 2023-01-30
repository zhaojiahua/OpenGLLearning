#include "GlobalFunctions.h"

int main()
{
	glfwInit();	//初始化GLFW
	//配置GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);	//创建多重采样样本缓冲(每个像素存储4个子样本)

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

	//加载模型数据
	ZModel testModel("assets/models/nanosuit/nanosuits.fbx");
	//ZModel testModel("assets/models/nanosuit/nanosuit.fbx");
	//ZModel testModel("assets/models/box.fbx");
	//ZModel testModel("assets/models/lowpolyscene/LowPolyWinterScene.obj");
	//ZModel testModel("assets/models/woodfloor/floor.fbx");
	//ZModel testModel("assets/models/woodfloor/pointlightScene2.fbx");
	ZModel testModel_square("assets/models/woodfloor/square.obj");
	//ZModel testModel_showMap("assets/models/woodfloor/showshadowmap.obj");
	//ZModel testModel("assets/models/boxes/boxes.fbx");
	//ZModel testModel("assets/models/ground/ground.obj");
	//ZModel testModel("assets/models/skybox/box.fbx");
	//ZModel testModel("assets/models/aixia/aixia.obj");
	ZModel testModel_Light("assets/models/lightsMod/pointL.obj");
	//cout << testModel.meshes[6].mTextures.size() << endl;
	//cout << testModel.meshes.size() << endl;

	//配置点光源(蓝色调)(为了便于观察效果我们把点光源衰减程度降低)
	vector<ZPointLight> pointlights;
	srand(7);
	for (unsigned int i = 0; i < 32; i++) {
		float posx = (rand() % 100 / 100.0f) * 80.0 - 40.0;
		float posy = (rand() % 100 / 100.0f) * 30.0 - 10.0;
		float posz = (rand() % 100 / 100.0f) * 80.0 - 30.0;
		float colorr = (rand() % 100 / 200.0f) + 0.5f;
		float colorg = (rand() % 100 / 200.0f) + 0.5f;
		float colorb = (rand() % 100 / 200.0f) + 0.5f;
		ZPointLight tempPointLight(glm::vec4(colorr, colorg, colorb, 1.0f), glm::vec3(posx, posy, posz), 1.0f, 0.03f, 0.01f);
		pointlights.push_back(tempPointLight);
	}

	//天空盒模型数据
	float skyboxVertices[] = {
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};
	unsigned int boxVAO, boxVBO;
	glGenBuffers(1, &boxVBO);
	glGenVertexArrays(1, &boxVAO);
	glBindVertexArray(boxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, boxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//创建天空盒子立方体贴图
	vector<string>faces{//贴图路径
		"assets/textures/skybox/right.jpg",
		"assets/textures/skybox/left.jpg",
		"assets/textures/skybox/top.jpg",
		"assets/textures/skybox/bottom.jpg",
		"assets/textures/skybox/front.jpg",
		"assets/textures/skybox/back.jpg",
	};
	unsigned int cubemap = LoadCubeMap(faces);

	//构建一个方片用来存放最终渲染的光照阶段图
	float rectVertices[] = { 
			-1.0f, 1.0f, 0.0f,		0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f,	0.0f, 0.0f,
			1.0f, 1.0f, 0.0f,		1.0f, 1.0f,
			1.0f, -1.0f, 0.0f,		1.0f, 0.0f,
	};
	unsigned int rectVAO, rectVBO;
	glGenBuffers(1, &rectVBO);
	glGenVertexArrays(1, &rectVAO);
	glBindVertexArray(rectVAO);
	glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectVertices), rectVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//创建着色器
	Shader myShader1_light("shaders/openGLLearning37/vertexShader_forlight.vs.c", "shaders/openGLLearning37/fragmentShader1_forlight.fs.c");//渲染光源用的
	Shader myShader1_skybox("shaders/openGLLearning23/vertexShader_skybox.vs.c", "shaders/openGLLearning23/fragmentShader1_skybox.fs.c");//渲染天空盒子用的
	Shader myShader1_square("shaders/openGLLearning41/vertexShader_square.vs.c", "shaders/openGLLearning41/fragmentShader1_square.fs.c");
	Shader myShader1_square2("shaders/openGLLearning41/vertexShader_square.vs.c", "shaders/openGLLearning41/fragmentShader1_square2.fs.c");
	Shader myShader1("shaders/openGLLearning41/vertexShader.vs.c", "shaders/openGLLearning41/fragmentShader1.fs.c");
	Shader myShader2("shaders/openGLLearning41/vertexShader2.vs.c", "shaders/openGLLearning41/fragmentShader2.fs.c");

	//设置采样器对应的纹理单元
	myShader1_skybox.Use();
	myShader1_skybox.SetInt("cubemap", 0);

	////////////////////////////////////////////////////////////////////////////////////生成一个gBuffer用来存储渲染得到的几何信息
	GLuint gBuffer;
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	//生成一张纹理附加颜色缓冲,存放位置缓冲数据
	GLuint gPosition, gNormal, gAlbedoSpec;
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, screenWidth, screenHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
	//生成一张纹理附加颜色缓冲,存放法线缓冲数据
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, screenWidth, screenHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
	//生成一张纹理附加颜色缓冲,存放漫反射和镜面颜色缓冲数据
	glGenTextures(1, &gAlbedoSpec);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);
	//告诉OpenGL我们将要使用哪种颜色附件进行渲染
	GLuint attachments[3] = { GL_COLOR_ATTACHMENT0 ,GL_COLOR_ATTACHMENT1 ,GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);
	//添加深度缓冲
	GLuint rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screenWidth, screenHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)cout << "framebuffer not complete" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	////////////////////////////////////////////////////////////////////////////////////生成一个FrameBuffer用来存储光照阶段渲染得到颜色信息
	GLuint finalBuffer;
	glGenFramebuffers(1, &finalBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, finalBuffer);
	GLuint diffusetex;
	glGenTextures(1,&diffusetex);
	glBindTexture(GL_TEXTURE_2D, diffusetex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, diffusetex, 0);
	//添加深度缓冲否则不完整
	GLuint depthtex;
	glGenRenderbuffers(1, &depthtex);
	glBindRenderbuffer(GL_RENDERBUFFER, depthtex);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screenWidth, screenHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthtex);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)cout << "framebuffer not complete" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClearColor(0.1f, 0.21f, 0.2f, 1.0f);	//设置颜色缓冲区的颜色值

	//设置四个方片的位置
	glm::mat4 squareMMs[5] = { glm::mat4(1.0f),glm::mat4(1.0f),glm::mat4(1.0f),glm::mat4(1.0f),glm::mat4(1.0f) };
	squareMMs[0] = glm::scale(squareMMs[0], glm::vec3(0.2f));
	squareMMs[0] = glm::translate(squareMMs[0], glm::vec3(-52.0f, 37.0f, -80.0f));
	squareMMs[1] = glm::scale(squareMMs[1], glm::vec3(0.2f));
	squareMMs[1] = glm::translate(squareMMs[1], glm::vec3(52.0f, 37.0f, -80.0f));
	squareMMs[2] = glm::scale(squareMMs[2], glm::vec3(0.2f));
	squareMMs[2] = glm::translate(squareMMs[2], glm::vec3(-52.0f, -37.0f, -80.0f));
	squareMMs[3] = glm::scale(squareMMs[3], glm::vec3(0.2f));
	squareMMs[3] = glm::translate(squareMMs[3], glm::vec3(52.0f, -37.0f, -80.0f));
	squareMMs[4] = glm::scale(squareMMs[4], glm::vec3(0.2f));
	squareMMs[4] = glm::translate(squareMMs[4], glm::vec3(0.0f, 0.0f, 40.0f));

	//保持渲染循环
	while (!glfwWindowShouldClose(window)) {	//如果检测到GLFW要求被关闭就结束循环
		ProcessInput(window);	//按键响应
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		zcamera.cameraMoveSpeed = 20.0f * deltaTime;

		//渲染指令

		//渲染场景的几何信息到gBuffer中
		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//清除默认帧缓冲区的颜色缓冲和深度缓冲
		glViewport(0, 0, screenWidth, screenHeight);//记得把视口分辨率设置回来
		////开启深度测试
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);//开启多重采样绘制(默认都是开启的)
		glEnable(GL_FRAMEBUFFER_SRGB);//开启gamma校正
		//渲染场景物体
		myShader1.Use();
		myShader1.SetMat4f("v_matrix", zcamera.GetCameraViewMatrix());
		myShader1.SetMat4f("p_matrix", zcamera.GetCameraperspectiveMatrix());
		testModel.DeferredDraw(&myShader1, glm::mat4(1.0f), zcamera.cameraPos, GL_TRIANGLES);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//复制gBuffer中的深度缓冲到finalBuffer中
		glBindFramebuffer(GL_READ_FRAMEBUFFER,gBuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER,finalBuffer);
		glBlitFramebuffer(0,0,screenWidth,screenHeight,0,0,screenWidth,screenHeight,GL_DEPTH_BUFFER_BIT,GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//渲染场景到finalBuffer中
		glBindFramebuffer(GL_FRAMEBUFFER, finalBuffer);
		glClear(GL_COLOR_BUFFER_BIT);	//清除默认帧缓冲区的颜色缓冲(注意这里不清除深度缓冲区)
		//渲染方片上的影像
		glDepthMask(GL_FALSE);//先禁止写入深度缓冲
		myShader2.Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gPosition);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gNormal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
		myShader2.SetInt("gPosition", 0);
		myShader2.SetInt("gNormal", 1);
		myShader2.SetInt("gAlbedoSpec", 2);
		myShader2.SetVec3f("viewPos", zcamera.cameraPos);
		for (unsigned int i = 0; i < 32; i++) {
			myShader2.SetVec3f("pointlights[" + to_string(i) + "].position", pointlights[i].mLightPosition);
			myShader2.SetVec4f("pointlights[" + to_string(i) + "].color", pointlights[i].mLightColor);
			myShader2.SetFloat("pointlights[" + to_string(i) + "].linear", pointlights[i].mKl);
			myShader2.SetFloat("pointlights[" + to_string(i) + "].quadratic", pointlights[i].mKq);
		}
		glBindVertexArray(rectVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
		glDepthMask(GL_TRUE);//渲染灯泡的时候开始允许写入深度缓冲
		//渲染灯泡
		myShader1_light.Use();
		myShader1_light.SetMat4f("v_matrix", zcamera.GetCameraViewMatrix());
		myShader1_light.SetMat4f("p_matrix", zcamera.GetCameraperspectiveMatrix());
		for (unsigned int i = 0; i < 32; i++) {
			myShader1_light.SetVec4f("lightColor", pointlights[i].mLightColor);
			glm::mat4 tempMM(1.0f);
			tempMM = glm::scale(tempMM, glm::vec3(0.5f));
			tempMM = glm::translate(tempMM, pointlights[i].mLightPosition);
			testModel_Light.Draw(&myShader1_light, tempMM,zcamera.cameraPos,GL_TRIANGLES);
		}
		//glEnable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);//场景渲染完成解绑浮点缓冲区

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//清除默认帧缓冲区的颜色缓冲和深度缓冲
		//使用一个方片把渲染的帧缓冲中的颜色纹理显示出来
		myShader1_square.Use();
		myShader1_square.SetMat4f("v_matrix", zcamera.GetCameraViewMatrix());
		myShader1_square.SetMat4f("p_matrix", zcamera.GetCameraperspectiveMatrix());
		//给四个方片分别设置diffuse贴图
		glBindTexture(GL_TEXTURE_2D, gPosition);
		testModel_square.Draw(&myShader1_square, squareMMs[0], zcamera.cameraPos, GL_TRIANGLES);
		glBindTexture(GL_TEXTURE_2D, gNormal);
		testModel_square.Draw(&myShader1_square, squareMMs[1], zcamera.cameraPos, GL_TRIANGLES);
		glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
		testModel_square.Draw(&myShader1_square, squareMMs[2], zcamera.cameraPos, GL_TRIANGLES);
		myShader1_square2.Use();
		myShader1_square2.SetMat4f("v_matrix", zcamera.GetCameraViewMatrix());
		myShader1_square2.SetMat4f("p_matrix", zcamera.GetCameraperspectiveMatrix());
		glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
		testModel_square.Draw(&myShader1_square2, squareMMs[3], zcamera.cameraPos, GL_TRIANGLES);
		//给第五个方片设置finaldiffuse贴图
		myShader1_square.Use();
		myShader1_square.SetMat4f("v_matrix", zcamera.GetCameraViewMatrix());
		myShader1_square.SetMat4f("p_matrix", zcamera.GetCameraperspectiveMatrix());
		glBindTexture(GL_TEXTURE_2D, diffusetex);
		testModel_square.Draw(&myShader1_square, squareMMs[4], zcamera.cameraPos, GL_TRIANGLES);

		//渲染天空盒子并且将深度写入关闭,这样盒子永远绘制在其他物体的背后
		glDepthFunc(GL_LEQUAL);
		myShader1_skybox.Use();
		glm::mat4 viewM = glm::mat4(glm::mat3(zcamera.GetCameraViewMatrix()));//去除相机的位移信息,只保留相机的旋转信息
		glUniformMatrix4fv(glGetUniformLocation(myShader1_skybox.ID, "v_matrix"), 1, GL_FALSE, glm::value_ptr(viewM));
		glUniformMatrix4fv(glGetUniformLocation(myShader1_skybox.ID, "p_matrix"), 1, GL_FALSE, glm::value_ptr(zcamera.GetCameraperspectiveMatrix()));
		glBindVertexArray(boxVAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthFunc(GL_LESS);

		glfwSwapBuffers(window);	//此函数会交换颜色缓冲(它是存储着GLFW窗口每一个像素颜色值的大缓冲),它在这一迭代中被用来绘制,并且会作为输出显示在屏幕上
		glfwPollEvents();	//此函数检查有没有鼠标键盘窗口等触发事件,如果有并调用相应的回调函数
	}
	//释放缓冲内存
	glDeleteVertexArrays(1, &boxVAO);
	glDeleteBuffers(1, &boxVBO);

	glfwTerminate();	//渲染结束后我们需要释放所有的分配资源,此函数可以完成
	return 0;
}

