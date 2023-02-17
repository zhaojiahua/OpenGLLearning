#pragma once

#include "Shader.h"
#include "Texture2D.h"

//一个方形的屏幕专门用来渲染中间Buffer里的内容
class ScreenRect
{
public:
	GLuint mVAO;
	Shader mShader;
	GLuint mTex;

	//开启特效
	GLboolean mConfuse, mChos, mShake;
	GLfloat mShakeTime = 0.0f;

	ScreenRect();
	ScreenRect(Shader& inshader);

	//想内存写入数据并配置VAO
	void InitRenderData();

	void Draw();

};

