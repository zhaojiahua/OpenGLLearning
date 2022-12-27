//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//
//#include <iostream>
//
//using namespace std;
//
////我们暂时将顶点着色器的源代码硬编码在C风格字符串中
//const char* vertexShaderSourceCode = "#version 330 core\n"
//	"layout(location = 0) in vec3 aPos;\n"	//location=0是标记了顶点的位置属性信息为0号
//	"void main()\n"
//	"{\n"
//	"	gl_Position = vec4(aPos.x , aPos.y , aPos.z , 1.0);\n"
//	"}\0";
////片元着色器源代码
//const char* fragmentShaderSourceCode1 = "#version 330 core\n"
//	"out vec4 fragColor;\n"
//	"void main()\n"
//	"{\n"
//	"	fragColor=vec4(1.0f,0.5f,0.3f,1.0f);\n"
//	"}\0";
////片元着色器源代码2
//const char* fragmentShaderSourceCode2 = "#version 330 core\n"
//"out vec4 fragColor;\n"
//"void main()\n"
//"{\n"
//"	fragColor=vec4(1.0f,1.0f,0.1f,1.0f);\n"
//"}\0";
//
////窗口大小发生变化时候的回调函数
//void Framebuffer_size_callback(GLFWwindow* inWind, int inWidth, int inHeight) {
//	glViewport(0, 20, inWidth, inHeight - 40);
//}
//
////输入响应函数
//void ProcessInput(GLFWwindow* inWind) {
//	if (glfwGetKey(inWind, GLFW_KEY_ESCAPE) == GLFW_PRESS) {//如果检测到ESC键被按下
//		glfwSetWindowShouldClose(inWind, true);	//设置glfwWindowShouldClose为TRUE
//	}
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
//	GLFWwindow* window = glfwCreateWindow(800, 600, "ZJHOpenGL", NULL, NULL);
//	if (window == nullptr) {
//		cout << "Failed to create GLFW window" << endl;
//		glfwTerminate();	//如果窗口创建失败,glfw停止工作,结束当前线程
//		return -1;
//	}
//	glfwMakeContextCurrent(window);	//将此窗口的上下文设置成当前线程的主上下文
//
//	//GLAD是用来管理OpenGL的函数指针的,所以在调用任何OpenGL的函数之前我们需要初始化GLAD
//	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
//		cout << "Failed to initialize GLAD" << endl;
//		return -1;
//	}
//
//	//视口
//	//在我们开始渲染之前还有一件重要的事情要做的就是设置渲染窗口的大小,即视口的大小(viewport),这样OpenGL知道怎么根据窗口的大小显示数据和坐标(通常我们设置视口比窗口小一点,这样可以在视口外窗口内显示一些其他的可视元素)
//	glViewport(0, 0, 800, 600);	//前两个参数是视口左下角的位置,后两个参数是视口的宽和高
//
//	//当用户改变窗口的大小的时候视口大小也要相应的变化,我们注册一个回调函数,它会在每次窗口大小发生变化的时候被调用
//	glfwSetFramebufferSizeCallback(window, Framebuffer_size_callback);	//注册这个函数,告诉GLFW每当窗口发小调整的时候调用这个函数(窗口第一次显示的时候这个函数也会被调用)
//
//	//自定义一个顶点数组
//	float vertices[] = {//一个三角形数据
//	0.5f, 0.5f, 0.0f,   // 右上角
//	0.5f, -0.5f, 0.0f,  // 右下角
//	-0.5f, -0.5f, 0.0f, // 左下角
//	-0.5f, 0.5f, 0.0f   // 左上角
//	};
//	float vertices_[] = {//一个三角形数据
//	0.5f, 0.5f, 0.0f,   // 右上角
//	0.5f, -0.5f, 0.0f,  // 右下角
//	-0.5f, -0.5f, 0.0f, // 左下角
//	-0.5f, 0.5f, 0.0f   // 左上角
//	};
//	float vertices2[] = {
//		//第一个三角形
//		-0.5f,0.5f,0.0f,
//		0.48f,0.5f,0.0f,
//		-0.5f,-0.48f,0.0f,
//		//第二个三角形
//		0.5f,0.48f,0.0f,
//		-0.48f,-0.5f,0.0f,
//		0.5f,-0.5f,0.0f
//	};
//
//	//顶点索引缓冲
//	unsigned int indices[] = {
//		0,1,3,	//第一个三角形
//		1,2,3		//第二个三角形
//	};
//
//	//创建一个顶点缓冲对象和顶点数组对象和一个元素缓冲对象
//	unsigned int EBO;
//	unsigned int VBOs[2];
//	unsigned int VAOs[2];
//	glGenBuffers(1, &EBO);
//	glGenBuffers(2, VBOs);
//	glGenVertexArrays(2, VAOs);
//
//	//绑定第一个VAO和VBO
//	//绑定VAO
//	glBindVertexArray(VAOs[0]);
//	//绑定VBO
//	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
//	//绑定EBO
//	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//	//填充数据
//	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
//	//设置顶点属性指针
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//	//解绑
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindVertexArray(0);
//
//	//绑定第二个VAO和VBO
//	//绑定VAO
//	glBindVertexArray(VAOs[1]);
//	//绑定VBO
//	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
//	//绑定EBO
//	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//	//填充数据
//	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
//	//设置顶点属性指针
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(9 * sizeof(float)));
//	glEnableVertexAttribArray(0);
//	//解绑
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindVertexArray(0);
//
//	//创建顶点着色器
//	unsigned int vertexShader;
//	vertexShader = glCreateShader(GL_VERTEX_SHADER);
//	//将着色器源码附着在着色器对象上并编译它
//	glShaderSource(vertexShader, 1, &vertexShaderSourceCode, NULL);
//	glCompileShader(vertexShader);
//	//检查是否编译成功
//	int success;
//	char infoLog[512];
//	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);	//此函数捕获着色器的编译成功与否
//	if (!success) {//如果编译失败,用glGetShaderInfoLog函数来捕获错误消息
//		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
//		cout << "ERROR:SHADER::VERTEX::COMPOILE_FAILED\n" << infoLog << endl;
//	}
//
//	//创建片元着色器
//	unsigned int fragmentShader1, fragmentShader2;
//	fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
//	fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
//	glShaderSource(fragmentShader1, 1, &fragmentShaderSourceCode1, NULL);
//	glShaderSource(fragmentShader2, 1, &fragmentShaderSourceCode2, NULL);
//	glCompileShader(fragmentShader1);
//	glCompileShader(fragmentShader2);
//	glGetShaderiv(fragmentShader1, GL_COMPILE_STATUS, &success);
//	if (!success) {
//		glGetShaderInfoLog(fragmentShader1, 512, NULL, infoLog);
//		cout << "ERROR:SHADER1::FRAGMENT::COMPOILE_FAILED\n" << infoLog << endl;
//	}
//	glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
//	if (!success) {
//		glGetShaderInfoLog(fragmentShader2, 512, NULL, infoLog);
//		cout << "ERROR:SHADER2::FRAGMENT::COMPOILE_FAILED\n" << infoLog << endl;
//	}
//
//	//常见着色器程序并将vertexShader和FragmentShader链接到此着色器程序中
//	//创建一个着色器程序对象
//	unsigned int shaderProgram1, shaderProgram2;
//	shaderProgram1 = glCreateProgram();
//	shaderProgram2 = glCreateProgram();
//	//链接着色器程序
//	glAttachShader(shaderProgram1, vertexShader);
//	glAttachShader(shaderProgram1, fragmentShader1);
//	glLinkProgram(shaderProgram1);
//	glAttachShader(shaderProgram2, vertexShader);
//	glAttachShader(shaderProgram2, fragmentShader2);
//	glLinkProgram(shaderProgram2);
//	//检测是否链接成功
//	glGetProgramiv(shaderProgram1, GL_LINK_STATUS, &success);
//	if (!success) {
//		glGetProgramInfoLog(shaderProgram1, 512, NULL, infoLog);
//		cout << "ERROR::SHADER1::PROGRAM::LINK_FAILED\n" << infoLog << endl;
//	}
//	glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
//	if (!success) {
//		glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
//		cout << "ERROR::SHADER2::PROGRAM::LINK_FAILED\n" << infoLog << endl;
//	}
//	//在把着色器对象链接到程序之后就可以删除着色器对象了,我们不再需要它了
//	glDeleteShader(vertexShader);
//	glDeleteShader(fragmentShader1);
//	glDeleteShader(fragmentShader2);
//
//	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	//开启线框绘制模式
//
//	//保持渲染循环
//	while (!glfwWindowShouldClose(window)) {	//如果检测到GLFW要求被关闭就结束循环
//		ProcessInput(window);	//每一循环都检测ESC是否被按下
//
//		//渲染指令
//		glClearColor(0.1f, 0.21f, 0.2f, 1.0f);	//设置颜色缓冲区的颜色值
//		glClear(GL_COLOR_BUFFER_BIT);	//此函数会用flClearColor里的颜色填充整个颜色缓冲区
//
//		//激活这个程序对象
//		glUseProgram(shaderProgram1);	//激活之后,每个着色器调用和渲染调用都会使用这个程序对象
//		glBindVertexArray(VAOs[0]);//每次绘制都绑定一次VAO
//		glDrawArrays(GL_TRIANGLES, 0, 3);
//		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//		glBindVertexArray(0);//解绑
//
//		glUseProgram(shaderProgram2);
//		glBindVertexArray(VAOs[1]);//每次绘制都绑定一次VAO
//		glDrawArrays(GL_TRIANGLES, 0, 3);
//		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//		glBindVertexArray(0);//解绑
//
//		glfwSwapBuffers(window);	//此函数会交换颜色缓冲(它是存储着GLFW窗口每一个像素颜色值的大缓冲),它在这一迭代中被用来绘制,并且会作为输出显示在屏幕上
//		glfwPollEvents();	//此函数检查有没有鼠标键盘窗口等触发事件,如果有并调用相应的回调函数
//	}
//
//	//结束之前释放VAO VBO和EBO资源
//	glDeleteBuffers(2, VBOs);
//	glDeleteBuffers(1, &EBO);
//	glDeleteVertexArrays(2, VAOs);
//	glDeleteProgram(shaderProgram1);
//	glDeleteProgram(shaderProgram2);
//
//	glfwTerminate();	//渲染结束后我们需要释放所有的分配资源,此函数可以完成
//	return 0;
//}
//
