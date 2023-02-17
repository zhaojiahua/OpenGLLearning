#pragma once
#include <glad/glad.h>
class Texture2D
{
public:
	GLuint ID;
	GLuint Width, Height;
	//纹理的格式
	GLuint Internal_format;
	//被加载的图片的格式
	GLuint Image_format;
	//纹理是否是浮点型存储方式(是否超过颜色值域)
	GLboolean floatType;
	//纹理属性参数配置
	GLuint Wrap_s;//S方向的UV覆盖方式
	GLuint Wrap_t;//T方向的UV覆盖方式
	GLuint Filter_min;//纹理采样过滤方式(纹理像素比屏幕分辨率小)
	GLuint Filter_mag;//纹理采样过滤方式(纹理像素比屏幕分辨率大)

	//构造函数
	Texture2D();
	//从硬盘中的图片中读取数据并生成纹理(以无符号整型数据存储)
	void Generate(GLuint width, GLuint height, unsigned char* data);
	//从硬盘中的图片中读取数据并生成纹理(以浮点型数据存储)
	void Generate_f(GLuint width, GLuint height, float* data);
	//绑定这个纹理到当前激活的GL_TEXTURE_2D纹理对象中
	void Bind() const;
	//从通道数量确定Internal_format和Image_format
	void SetFormat(GLint channelCount);
};

