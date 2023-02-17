#include "GlobalFunctions_Game.h"

int main() {
	glfwInit();//初始化glfw
	//配置OpenGL版本等
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//主版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//次版本号
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//使用核心版本
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);//避免窗口闪退

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "ZJHBreakout", nullptr, nullptr);//创建窗口对象
	glfwMakeContextCurrent(window);//将此窗口设定为当前上下文对象

	//GLAD是用来管理OpenGL的函数指针的,所以在调用任何OpenGL的函数之前我们需要初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	//设置窗口的回调函数
	glfwSetFramebufferSizeCallback(window, Framebuffer_size_callback);//窗口变换回调函数
	glfwSetCursorPosCallback(window, Mouse_move_callback);//移动鼠标回调函数
	glfwSetScrollCallback(window, Mouse_scroll_callback);//鼠标滚动回调函数
	glfwSetKeyCallback(window, Key_callback);//按键响应回调函数

	glViewport(0, 0, screenWidth, screenHeight);	//设置窗口中视图的分辨率大小(与窗口一致)
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	//初始化游戏实例
	BreakoutGame.Init();
	BreakoutGame.State = GAME_ACTIVE;
	BreakoutGame.GenInterimBuffer();
	BreakoutGame.SetShaderBeforeRender();
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;
	//未检测到窗口关闭就一直保持游戏循环
	while (!glfwWindowShouldClose(window)) {
		//计算每帧之间的时间间隔
		GLfloat currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		glfwPollEvents();//只处理已经在消息队列中的消息并立即返回结果
		//处理输入函数
		BreakoutGame.ProcessInput(deltaTime);
		//更新游戏状态
		BreakoutGame.Updata(deltaTime);
		//渲染
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT || GL_DEPTH_BUFFER_BIT);
		glBindFramebuffer(GL_FRAMEBUFFER, BreakoutGame.interimBuffer);
		BreakoutGame.Render();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT || GL_DEPTH_BUFFER_BIT);
		BreakoutGame.RenderRect();
		
		//交换缓冲
		glfwSwapBuffers(window);
	}
	//循环结束后清理资源并关闭窗口
	ResourceManager::Clear();
	glfwTerminate();
	return 0;
}