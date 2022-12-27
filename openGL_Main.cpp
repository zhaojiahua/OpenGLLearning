#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

using namespace std;

//窗口大小发生变化时候的回调函数
void Framebuffer_size_callback(GLFWwindow* inWind, int inWidth, int inHeight) {
	glViewport(0, 20, inWidth, inHeight - 40);
}

//输入响应函数
void ProcessInput(GLFWwindow* inWind) {
	if (glfwGetKey(inWind, GLFW_KEY_ESCAPE) == GLFW_PRESS) {//如果检测到ESC键被按下
		glfwSetWindowShouldClose(inWind, true);	//设置glfwWindowShouldClose为TRUE
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
	GLFWwindow* window = glfwCreateWindow(800, 600, "ZJHOpenGL", NULL, NULL);
	if (window == nullptr) {
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();	//如果窗口创建失败,glfw停止工作,结束当前线程
		return -1;
	}
	glfwMakeContextCurrent(window);	//将此窗口的上下文设置成当前线程的主上下文

	//GLAD是用来管理OpenGL的函数指针的,所以在调用任何OpenGL的函数之前我们需要初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	//视口
	//在我们开始渲染之前还有一件重要的事情要做的就是设置渲染窗口的大小,即视口的大小(viewport),这样OpenGL知道怎么根据窗口的大小显示数据和坐标(通常我们设置视口比窗口小一点,这样可以在视口外窗口内显示一些其他的可视元素)
	glViewport(0, 20, 800, 560);	//前两个参数是视口左下角的位置,后两个参数是视口的宽和高

	//当用户改变窗口的大小的时候视口大小也要相应的变化,我们注册一个回调函数,它会在每次窗口大小发生变化的时候被调用
	glfwSetFramebufferSizeCallback(window, Framebuffer_size_callback);	//注册这个函数,告诉GLFW每当窗口发小调整的时候调用这个函数(窗口第一次显示的时候这个函数也会被调用)

	//保持渲染循环
	while (!glfwWindowShouldClose(window)) {	//如果检测到GLFW要求被关闭就结束循环
		ProcessInput(window);	//每一循环都检测ESC是否被按下

		//渲染指令
		glClearColor(0.1f, 0.21f, 0.2f, 1.0f);	//设置颜色缓冲区的颜色值
		glClear(GL_COLOR_BUFFER_BIT);	//此函数会用flClearColor里的颜色填充整个颜色缓冲区

		glfwSwapBuffers(window);	//此函数会交换颜色缓冲(它是存储着GLFW窗口每一个像素颜色值的大缓冲),它在这一迭代中被用来绘制,并且会作为输出显示在屏幕上
		glfwPollEvents();	//此函数检查有没有鼠标键盘窗口等触发事件,如果有并调用相应的回调函数
	}

	glfwTerminate();	//渲染结束后我们需要释放所有的分配资源,此函数可以完成
	return 0;
}

