#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include "Shader.h"

using namespace std;


//窗口大小发生变化时候的回调函数
void Framebuffer_size_callback(GLFWwindow* inWind, int inWidth, int inHeight) {
	glViewport(0, 20, inWidth, inHeight - 40);
}

//输入响应函数
void ProcessInput(GLFWwindow* inWind,Shader inshader) {
	if (glfwGetKey(inWind, GLFW_KEY_ESCAPE) == GLFW_PRESS) {//如果检测到ESC键被按下
		glfwSetWindowShouldClose(inWind, true);	//设置glfwWindowShouldClose为TRUE
	}
	if (glfwGetKey(inWind, GLFW_KEY_UP) == GLFW_PRESS) {//如果检测到up键被按下
		float tempvalue; 
		glGetUniformfv(inshader.ID, glGetUniformLocation(inshader.ID, "mixValue"), &tempvalue);
		tempvalue += 0.0002;
		if (tempvalue > 1.0f) tempvalue = 1.0f;
		inshader.SetFloat("mixValue", tempvalue);
	}
	if (glfwGetKey(inWind, GLFW_KEY_DOWN) == GLFW_PRESS) {//如果检测到down键被按下
		float tempvalue;
		glGetUniformfv(inshader.ID, glGetUniformLocation(inshader.ID, "mixValue"), &tempvalue);
		tempvalue -= 0.0002;
		if (tempvalue < 0.0f) tempvalue = 0.0f;
		inshader.SetFloat("mixValue", tempvalue);
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
	glViewport(0, 0, 800, 600);	//前两个参数是视口左下角的位置,后两个参数是视口的宽和高

	//当用户改变窗口的大小的时候视口大小也要相应的变化,我们注册一个回调函数,它会在每次窗口大小发生变化的时候被调用
	glfwSetFramebufferSizeCallback(window, Framebuffer_size_callback);	//注册这个函数,告诉GLFW每当窗口发小调整的时候调用这个函数(窗口第一次显示的时候这个函数也会被调用)

	//自定义一个顶点数组
	float vertices[] = {//一个三角形数据
	0.5f, 0.5f, 0.0f,			1.0f,0.0f,0.0f,	4.0f,4.0f,		// 右上角
	0.5f, -0.5f, 0.0f,			0.0f,1.0f,0.0f,	4.0f,0.0f,		// 右下角
	-0.5f, -0.5f, 0.0f,		0.0f,0.0f,1.0f,	0.0f,0.0f,		// 左下角
	-0.5f, 0.5f, 0.0f,			1.0f,1.0f,1.0f,	0.0f,4.0f		// 左上角
	};

	//顶点索引缓冲
	unsigned int indices[] = {
		0,1,3,	//第一个三角形
		1,2,3		//第二个三角形
	};

	//创建一个顶点缓冲对象和顶点数组对象和一个元素缓冲对象
	unsigned int EBO;
	unsigned int VBO;
	unsigned int VAO;
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	//绑定第一个VAO和VBO
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

	//创建顶点着色器
	Shader myShader1("shaders/vertexShader.vs", "shaders/fragmentShader1.fs");
	
	//纹理
	//生成2个纹理对象
	unsigned int textures[2];
	glGenTextures(1, &textures[0]);
	//绑定纹理1
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	//设置当前绑定的纹理对象的环绕和过滤方式
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//读取纹理图片
	int tex_width, tex_height, tex_channelsNum;
	unsigned char* tex_data = stbi_load("assets/textures/mx.jpg", &tex_width, &tex_height, &tex_channelsNum, 0);
	//使用glTexImage2D函数将图片数据生成纹理
	if (tex_data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_data);
		glGenerateMipmap(GL_TEXTURE_2D);	//这会为当前绑定的纹理自动生成所需要的多级渐进纹理
	}
	else cout << "Failed to load texture1" << endl;

	//绑定纹理2
	stbi_set_flip_vertically_on_load(true);//加载图片的时候翻转Y坐标
	tex_data = stbi_load("assets/textures/face.png", &tex_width, &tex_height, &tex_channelsNum, 0);
	glGenTextures(1, &textures[1]);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	//设置当前绑定的纹理对象的环绕和过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//使用glTexImage2D函数将图片数据生成纹理
	if (tex_data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
		glGenerateMipmap(GL_TEXTURE_2D);	//这会为当前绑定的纹理自动生成所需要的多级渐进纹理
	}
	else cout << "Failed to load texture2" << endl;
	stbi_image_free(tex_data);	//生成纹理后释放data内存

	//设置每个采样器对应的纹理单元
	myShader1.Use();
	myShader1.SetInt("myTexture1", 0);
	myShader1.SetInt("myTexture2", 1);

	//保持渲染循环
	while (!glfwWindowShouldClose(window)) {	//如果检测到GLFW要求被关闭就结束循环
		ProcessInput(window,myShader1);	//按键响应

		//渲染指令
		glClearColor(0.1f, 0.21f, 0.2f, 1.0f);	//设置颜色缓冲区的颜色值
		glClear(GL_COLOR_BUFFER_BIT);	//此函数会用flClearColor里的颜色填充整个颜色缓冲区

		//激活这个程序对象
		myShader1.Use();
		glActiveTexture(GL_TEXTURE0);//激活第0号采样器(默认被激活)
		glBindTexture(GL_TEXTURE_2D, textures[0]);//绑定第一个texture到第0号采样器
		glActiveTexture(GL_TEXTURE1);//激活第1号采样器
		glBindTexture(GL_TEXTURE_2D, textures[1]);	//绑定第二个texture到第1号采样器
		glBindVertexArray(VAO);//每次绘制都绑定一次VAO
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);//解绑

		glfwSwapBuffers(window);	//此函数会交换颜色缓冲(它是存储着GLFW窗口每一个像素颜色值的大缓冲),它在这一迭代中被用来绘制,并且会作为输出显示在屏幕上
		glfwPollEvents();	//此函数检查有没有鼠标键盘窗口等触发事件,如果有并调用相应的回调函数
	}

	//结束之前释放VAO VBO和EBO资源
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);

	glfwTerminate();	//渲染结束后我们需要释放所有的分配资源,此函数可以完成
	return 0;
}

