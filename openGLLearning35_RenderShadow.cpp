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
//	ZModel testModel("assets/models/woodfloor/floor.fbx");
//	//ZModel testModel_showMap("assets/models/woodfloor/showshadowmap.obj");
//	//ZModel testModel("assets/models/boxes/boxes.fbx");
//	//ZModel testModel("assets/models/ground/ground.obj");
//	//ZModel testModel("assets/models/skybox/box.fbx");
//	//ZModel testModel("assets/models/aixia/aixia.obj");
//	ZModel testModel_DirLight("assets/models/directionLight/dirL.obj");
//
//	//配置平行光源(红色调)
//	ZDirectionLight directionLight(glm::vec4(0.9f, 0.85f, 0.8f, 1.0f), glm::vec3(-1.0f, -0.5f, -0.2f));
//	directionLight.mLightPosition = glm::vec3(5.0f, 10.0f, -5.0f);//给平行光一个位置
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
//	//创建着色器
//	Shader myShader1_light("shaders/openGLLearning34/vertexShader_forlight.vs.c", "shaders/openGLLearning34/fragmentShader1_forlight.fs.c");//渲染平行光箭头用的
//	Shader myShader1_skybox("shaders/openGLLearning23/vertexShader_skybox.vs.c", "shaders/openGLLearning23/fragmentShader1_skybox.fs.c");//渲染天空盒子用的
//	Shader myShader1_shadowmap("shaders/openGLLearning35/vertexShader_forshadowdepth.vs.c", "shaders/openGLLearning35/fragmentShader1_forshadowdepth.fs.c");//渲染阴影深度贴图用的
//	Shader myShader1("shaders/openGLLearning35/vertexShader.vs.c", "shaders/openGLLearning35/fragmentShader1.fs.c");
//
//	//设置采样器对应的纹理单元
//	myShader1.Use();
//	myShader1.SetFloat("material.shininess", 64.0f);
//	//把灯光信息传递给shader
//	directionLight.SetLight(&myShader1, 0, 0.1f, 0.5f, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));
//
//	myShader1_skybox.Use();
//	myShader1_skybox.SetInt("cubemap", 0);
//
//	glm::mat4 lightMM(1.0f);
//	glm::vec3 rotAxis = glm::normalize(glm::cross(glm::vec3(-1.0f, 0.0f, 0.0f), directionLight.mLightDirection));//旋转轴
//	float anglevalue = glm::acos(glm::dot(glm::vec3(-1.0f, 0.0f, 0.0f), glm::normalize(directionLight.mLightDirection)));//旋转角度
//	lightMM = glm::rotate(lightMM, anglevalue, rotAxis);
//	lightMM = glm::translate(lightMM, directionLight.mLightPosition);
//
//	myShader1_light.Use();
//	myShader1_light.SetVec4f("lightColor", directionLight.mLightColor);
//	myShader1_light.SetVec3f("lightDirection", directionLight.mLightDirection);
//
//	//从光源视角渲染阴影深度贴图
//	//从光源角度的相机投影矩阵(对于平行光我们用正交投影矩阵)
//	glm::mat4 perspectiveMM = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, 0.2f, 100.0f);
//	//从光源角度的相机视图矩阵(从光的反方向的一个位置看向场景中央)
//	glm::mat4 viewMM = glm::lookAt(-20.0f * directionLight.mLightDirection, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//	glm::mat4 lightSpaceMatrix = perspectiveMM * viewMM;//光空间变换矩阵
//	myShader1_shadowmap.Use();
//	myShader1_shadowmap.SetMat4f("lightspace_matrix", lightSpaceMatrix);
//	myShader1.Use();
//	myShader1.SetMat4f("lightspace_matrix", lightSpaceMatrix);
//
//	//生成一个帧缓冲来存储渲染的深度贴图
//	unsigned int shadowDepthMapFBO;
//	glGenFramebuffers(1, &shadowDepthMapFBO);
//	//生成一个2D纹理提供给帧缓冲的深度缓冲使用
//	const GLuint shadowW = depthRes, shadowH = depthRes;//阴影深度贴图的分辨率
//	unsigned int depthMap;
//	glGenTextures(1, &depthMap);
//	glBindTexture(GL_TEXTURE_2D, depthMap);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowW, shadowH, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//	//把生成的深度纹理绑定到帧缓冲的深度缓冲
//	glBindFramebuffer(GL_FRAMEBUFFER, shadowDepthMapFBO);
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
//	glDrawBuffer(GL_NONE);//我们只渲染深度缓冲,对于颜色方面的缓冲每个帧缓冲都是默认自带的,这里不做操作即可
//	glReadBuffer(GL_NONE);
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
//		//渲染深度贴图
//		glViewport(0, 0, shadowW, shadowH);//注意设置视口分辨率等于深度贴图分辨率
//		glBindFramebuffer(GL_FRAMEBUFFER, shadowDepthMapFBO);//渲染深度到指定的帧缓冲
//		glClear(GL_DEPTH_BUFFER_BIT);//每一帧清除当前帧缓冲的深度缓冲
//		glCullFace(GL_FRONT);//为解决彼得潘悬浮问题我们把正面裁减掉保留背面渲染阴影
//		myShader1_shadowmap.Use();
//		testModel.DrawDepth(&myShader1_shadowmap);
//		glBindFramebuffer(GL_FRAMEBUFFER, 0);
//		glCullFace(GL_BACK);
//
//		//渲染场景
//		glClearColor(0.1f, 0.21f, 0.2f, 1.0f);	//设置颜色缓冲区的颜色值
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//清除默认帧缓冲区的颜色缓冲和深度缓冲
//		glViewport(0, 0, screenWidth, screenHeight);//记得把视口分辨率设置回来
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
//		//开启深度测试
//		glEnable(GL_DEPTH_TEST);
//		glEnable(GL_MULTISAMPLE);//开启多重采样绘制(默认都是开启的)
//		glEnable(GL_FRAMEBUFFER_SRGB);//开启gamma校正
//		//渲染场景物体
//		myShader1.Use();
//		myShader1.SetVec3f("viewPos", zcamera.cameraPos);
//		myShader1.SetMat4f("v_matrix", zcamera.GetCameraViewMatrix());
//		myShader1.SetMat4f("p_matrix", zcamera.GetCameraperspectiveMatrix());
//		testModel.DrawWithShadow(&myShader1, glm::mat4(1.0f), zcamera.cameraPos, GL_TRIANGLES, depthMap);
//
//		//渲染平行光物体
//		myShader1_light.Use();
//		myShader1_light.SetVec3f("viewPos", zcamera.cameraPos);
//		myShader1_light.SetMat4f("v_matrix", zcamera.GetCameraViewMatrix());
//		myShader1_light.SetMat4f("p_matrix", zcamera.GetCameraperspectiveMatrix());
//		testModel_DirLight.Draw(&myShader1_light, lightMM, zcamera.cameraPos, GL_TRIANGLES);
//
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
