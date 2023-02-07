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
//	//ZModel testModel("assets/models/nanosuit/nanosuits.fbx");
//	//ZModel testModel("assets/models/nanosuit/nanosuit.fbx");
//	//ZModel testModel("assets/models/nanosuit/nanosuit_plane.fbx");
//	//ZModel testModel("assets/models/pbrpokeball/pokeball2.fbx");
//	//ZModel testModel("assets/models/pe/pe_main.obj");
//	//ZModel testModel("assets/models/pbrmetals/sphere.obj");
//	ZModel testModel("assets/models/tools/tool.obj");
//	ZModel testModel_box("assets/models/box.obj");
//	//ZModel testModel("assets/models/lowpolyscene/LowPolyWinterScene.obj");
//	//ZModel testModel("assets/models/woodfloor/floor.fbx");
//	//ZModel testModel("assets/models/woodfloor/pointlightScene2.fbx");
//	ZModel testModel_square("assets/models/woodfloor/square.obj");
//	//ZModel testModel_showMap("assets/models/woodfloor/showshadowmap.obj");
//	//ZModel testModel("assets/models/boxes/boxes.fbx");
//	//ZModel testModel("assets/models/ground/ground.obj");
//	//ZModel testModel("assets/models/skybox/box.fbx");
//	//ZModel testModel("assets/models/aixia/aixia.obj");
//	ZModel testModel_Light("assets/models/lightsMod/pointL.obj");
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
//	//创建PBR贴图5张
//	GLuint albedomap, normalmap, metallicmap, roughnessmap, aomap;
//	albedomap = testModel.meshes[0].TextureFromFile("assets/models/tools/textures/Main_albedo.png", true);
//	normalmap = testModel.meshes[0].TextureFromFile("assets/models/tools/textures/Main_normal.png");
//	metallicmap = testModel.meshes[0].TextureFromFile("assets/models/tools/textures/Main_metallic.png");
//	roughnessmap = testModel.meshes[0].TextureFromFile("assets/models/tools/textures/Main_roughness.png");
//	aomap = testModel.meshes[0].TextureFromFile("assets/models/tools/textures/Main_ao.png");
//
//	//hdr贴图(等距柱状投影图)
//	GLuint hdrmap = testModel_box.meshes[0].TextureFromFile_f("assets/models/4KHDRIS/1.hdr");
//
//	//创建着色器
//	Shader myShader1_light("shaders/openGLLearning37/vertexShader_forlight.vs.c", "shaders/openGLLearning37/fragmentShader1_forlight.fs.c");//渲染光源用的
//	Shader myShader1_skybox("shaders/openGLLearning23/vertexShader_skybox.vs.c", "shaders/openGLLearning23/fragmentShader1_skybox.fs.c");//渲染天空盒子用的
//	Shader myShader1_square("shaders/openGLLearning41/vertexShader_square.vs.c", "shaders/openGLLearning41/fragmentShader1_square.fs.c");
//	Shader myShader2("shaders/openGLLearning44/vertexShader.vs.c", "shaders/openGLLearning44/fragmentShader2.fs.c");
//	Shader myShader2_hdr2cubemap("shaders/openGLLearning44/vertexShader_hdr2cubemap.vs.c", "shaders/openGLLearning44/fragmentShader1_hdr2cubemap.fs.c");
//	Shader myShader2_irradiancemap("shaders/openGLLearning44/vertexShader_hdr2cubemap.vs.c", "shaders/openGLLearning44/fragmentShader1_irradiancemap.fs.c");
//
//	//设置采样器对应的纹理单元
//	myShader1_skybox.Use();
//	myShader1_skybox.SetInt("cubemap", 0);
//	glm::mat4 squareMM = glm::mat4(1.0f);
//	squareMM = glm::translate(squareMM, glm::vec3(0.0f, 0.0f, -50.0f));
//
//	//灯光位置
//	glm::vec3 lightPositions[4];
//	lightPositions[0] = glm::vec3(5.0f, 4.0f, 50.0f);
//	lightPositions[1] = glm::vec3(5.0f, -4.0f, 50.0f);
//	lightPositions[2] = glm::vec3(-5.0f, 4.0f, 50.0f);
//	lightPositions[3] = glm::vec3(-5.0f, -4.0f, 50.0f);
//	//配置myShader2
//	myShader2.Use();
//	for (GLuint i = 0; i < 4; i++) {
//		myShader2.SetVec3f("lightPositions[" + to_string(i) + "]", lightPositions[i]);
//	}
//	////////////////////////////////////////////////////////////////////////////////////生成一个FrameBuffer用来存储HDR贴图映射到Cube贴图的六张纹理
//	GLuint hdrFBO, hdrRBO;
//	glGenFramebuffers(1,&hdrFBO);
//	glGenRenderbuffers(1,&hdrRBO);
//	glBindFramebuffer(GL_FRAMEBUFFER,hdrFBO);
//	glBindRenderbuffer(GL_RENDERBUFFER,hdrRBO);
//	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 1024, 1024);//设置纹理分辨率为1024X1024
//	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, hdrRBO);
//	GLuint cubeMap_forhdr;
//	glGenTextures(1, &cubeMap_forhdr);
//	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap_forhdr);
//	for (GLuint i = 0; i < 6; i++) {//生成盒子的六张贴图
//		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 1024, 1024, 0, GL_RGB, GL_FLOAT, nullptr);
//	}
//	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
//	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)cout << "hdrBuffer not complete" << endl;
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//	//为相机创建面向六个方向的视图矩阵
//	glm::mat4 hdrcameraPM = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);
//	glm::mat4 hdrcameraVM[6] = {//注意顺序是-----------------------------------------右 左 上 下 前 后
//		glm::lookAt(glm::vec3(0.0f),glm::vec3(1.0f,0.0f,0.0f),glm::vec3(0.0f,-1.0f,0.0f)),
//		glm::lookAt(glm::vec3(0.0f),glm::vec3(-1.0f,0.0f,0.0f),glm::vec3(0.0f,-1.0f,0.0f)),
//		glm::lookAt(glm::vec3(0.0f),glm::vec3(0.0f,1.0f,0.0f),glm::vec3(0.0f,0.0f,1.0f)),
//		glm::lookAt(glm::vec3(0.0f),glm::vec3(0.0f,-1.0f,0.0f),glm::vec3(0.0f,0.0f,-1.0f)),
//		glm::lookAt(glm::vec3(0.0f),glm::vec3(0.0f,0.0f,1.0f),glm::vec3(0.0f,-1.0f,0.0f)),
//		glm::lookAt(glm::vec3(0.0f),glm::vec3(0.0f,0.0f,-1.0f),glm::vec3(0.0f,-1.0f,0.0f))
//	};
//
//	////////////////////////////////////////////////////////////////////////////////////生成一个FrameBuffer用来存储光照阶段渲染得到颜色信息
//	GLuint finalBuffer;
//	glGenFramebuffers(1, &finalBuffer);
//	glBindFramebuffer(GL_FRAMEBUFFER, finalBuffer);
//	GLuint diffusetex;
//	glGenTextures(1, &diffusetex);
//	glBindTexture(GL_TEXTURE_2D, diffusetex);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glBindTexture(GL_TEXTURE_2D, 0);
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, diffusetex, 0);
//	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)cout << "finalBuffer not complete" << endl;
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	glClearColor(0.1f, 0.21f, 0.3f, 1.0f);	//设置颜色缓冲区的颜色值
//
//	//渲染hdr等距柱状纹理图到盒子纹理(这个渲染一定要放在循环外面)
//	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
//	glViewport(0, 0, 1024, 1024);//把视口分辨率设置1024X11024
//	myShader2_hdr2cubemap.Use();
//	myShader2_hdr2cubemap.SetInt("hdrmap", 0);
//	myShader2_hdr2cubemap.SetMat4f("p_matrix", hdrcameraPM);
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, hdrmap);
//	for (GLuint i = 0; i < 6; i++) {
//		myShader2_hdr2cubemap.SetMat4f("v_matrix", hdrcameraVM[i]);
//		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubeMap_forhdr, 0);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//		testModel_box.SampleDraw(&myShader2_hdr2cubemap);
//	}
//	glBindTexture(GL_TEXTURE_2D, 0);
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//	////////////////////////////////////////////////////////////////////////////////////生成一个立方体贴图用来存储辐照度
//	GLuint irradianceMap;
//	glGenTextures(1, &irradianceMap);
//	glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
//	for (GLuint i = 0; i < 6; i++) {//辐照度贴图是对半球内所有方向的积分,它丢失了大部分的高频细节,所以我们可以使用较低的分辨率
//		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 64, 64, 0, GL_RGB, GL_FLOAT, nullptr);
//	}
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
//	glBindRenderbuffer(GL_RENDERBUFFER,hdrRBO);
//	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 64, 64);
//	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)cout << "hdrBuffer not complete" << endl;
//
//	//使用卷积着色器对hdr环境贴图卷积,并将结果渲染到irradianceMap中
//	glViewport(0, 0, 64, 64);//把视口分辨率设置64X64
//	myShader2_irradiancemap.Use();
//	myShader2_irradiancemap.SetInt("hdrcubemap", 0);
//	myShader2_irradiancemap.SetMat4f("p_matrix", hdrcameraPM);
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap_forhdr);
//	for (GLuint i = 0; i < 6; i++) {
//		myShader2_irradiancemap.SetMat4f("v_matrix", hdrcameraVM[i]);
//		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//		testModel_box.SampleDraw(&myShader2_irradiancemap);
//	}
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//	//保持渲染循环
//	while (!glfwWindowShouldClose(window)) {	//如果检测到GLFW要求被关闭就结束循环
//		ProcessInput(window);	//按键响应
//		float currentFrame = glfwGetTime();
//		deltaTime = currentFrame - lastFrame;
//		lastFrame = currentFrame;
//		zcamera.cameraMoveSpeed = 20.0f * deltaTime;
//
//		//渲染指令
//
//		////开启深度测试
//		glViewport(0, 0, screenWidth, screenHeight);//完事记得把视口分辨率设置回来
//		glEnable(GL_DEPTH_TEST);
//		glEnable(GL_MULTISAMPLE);//开启多重采样绘制(默认都是开启的)
//		glEnable(GL_FRAMEBUFFER_SRGB);//开启gamma校正
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//清除默认帧缓冲区的颜色缓冲和深度缓冲
//		//渲染方片上的影像
//		myShader2.Use();
//		myShader2.SetVec3f("viewPos", zcamera.cameraPos);
//		myShader2.SetMat4f("v_matrix", zcamera.GetCameraViewMatrix());
//		myShader2.SetMat4f("p_matrix", zcamera.GetCameraperspectiveMatrix());
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, albedomap);
//		glActiveTexture(GL_TEXTURE1);
//		glBindTexture(GL_TEXTURE_2D, normalmap);
//		glActiveTexture(GL_TEXTURE2);
//		glBindTexture(GL_TEXTURE_2D, metallicmap);
//		glActiveTexture(GL_TEXTURE3);
//		glBindTexture(GL_TEXTURE_2D, roughnessmap);
//		glActiveTexture(GL_TEXTURE4);
//		glBindTexture(GL_TEXTURE_2D, aomap);
//		myShader2.SetInt("texture_albedo", 0);
//		myShader2.SetInt("texture_normal", 1);
//		myShader2.SetInt("texture_metallic", 2);
//		myShader2.SetInt("texture_roughness", 3);
//		myShader2.SetInt("texture_ao", 4);
//		testModel.SampleDraw(&myShader2);
//
//		//渲染天空盒子并且将深度写入关闭,这样盒子永远绘制在其他物体的背后
//		glDepthFunc(GL_LEQUAL);
//		myShader1_skybox.Use();
//		glm::mat4 viewM = glm::mat4(glm::mat3(zcamera.GetCameraViewMatrix()));//去除相机的位移信息,只保留相机的旋转信息
//		glUniformMatrix4fv(glGetUniformLocation(myShader1_skybox.ID, "v_matrix"), 1, GL_FALSE, glm::value_ptr(viewM));
//		glUniformMatrix4fv(glGetUniformLocation(myShader1_skybox.ID, "p_matrix"), 1, GL_FALSE, glm::value_ptr(zcamera.GetCameraperspectiveMatrix()));
//		glBindVertexArray(boxVAO);
//		glActiveTexture(GL_TEXTURE0);
//		//glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
//		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap_forhdr);
//		//glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
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
