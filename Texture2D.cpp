#include "Texture2D.h"

Texture2D::Texture2D(): Width(0),Height(0),Internal_format(GL_RGB),Image_format(GL_RGB),Wrap_s(GL_REPEAT),Wrap_t(GL_REPEAT),Filter_min(GL_LINEAR),Filter_mag(GL_LINEAR)
{
	glGenTextures(1, &ID);
}

void Texture2D::Generate(GLuint width, GLuint height, unsigned char* data)
{
	Width = width;
	Height = height;
	glBindTexture(GL_TEXTURE_2D, ID);//绑定这个纹理ID,然后下面的操作都是对它执行的
	glTexImage2D(GL_TEXTURE_2D, 0, Internal_format, width, height, 0, Image_format, GL_UNSIGNED_BYTE, data);
	//设置纹理属性
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Wrap_t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Filter_min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Filter_mag);
	glBindTexture(GL_TEXTURE_2D, 0);//设置完属性解绑
}

void Texture2D::Generate_f(GLuint width, GLuint height, float* data)
{
	Width = width;
	Height = height;
	glBindTexture(GL_TEXTURE_2D, ID);//绑定这个纹理ID,然后下面的操作都是对它执行的
	glTexImage2D(GL_TEXTURE_2D, 0, Internal_format, width, height, 0, Image_format, GL_FLOAT, data);
	//设置纹理属性
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Wrap_t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Filter_min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Filter_mag);
	glBindTexture(GL_TEXTURE_2D, 0);//设置完属性解绑
}

void Texture2D::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, ID);//绑定这个纹理ID,然后下面的操作都是对这个纹理执行的
}

void Texture2D::SetFormat(GLint channelCount)
{
	if (channelCount == 4) {
		this->Internal_format = GL_RGBA;
		this->Image_format = GL_RGBA;
	}
	else {
		this->Internal_format = GL_RGB;
		this->Image_format = GL_RGB;
	}
}
