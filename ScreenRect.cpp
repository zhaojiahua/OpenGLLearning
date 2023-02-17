#include "ScreenRect.h"
#include <GLFW/glfw3.h>

ScreenRect::ScreenRect()
{}

ScreenRect::ScreenRect(Shader & inshader):	mShader(inshader), mConfuse(GL_FALSE), mChos(GL_FALSE), mShake(GL_FALSE)
{
}

void ScreenRect::InitRenderData()
{
	//一个NDC空间的边长为2的四边形
	GLfloat vertices[] = {
		//position	//uv
			-1,-1,		0,0,
			  1,1,			1,1,
			  -1,1,		0,1,

			-1,-1,		0,0,
			 1,-1,		1,0,
			  1,1,			1,1
	};
	GLuint mVBO;
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindVertexArray(mVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	//解绑
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void ScreenRect::Draw()
{
	mShader.Use();
	//mShader.SetBool("chaos", GL_TRUE);
	mShader.SetBool("shake", mShake);
	mShader.SetBool("chaos", mChos);
	mShader.SetBool("confuse", mConfuse);
	mShader.SetFloat("time", glfwGetTime());
	glBindTexture(GL_TEXTURE_2D, mTex);
	glBindVertexArray(mVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}
