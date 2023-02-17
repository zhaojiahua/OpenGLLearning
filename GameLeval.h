#pragma once

#include <vector>

#include "GameObject.h"
#include "SpriteRender.h"

//关卡基本上由砖块组成,因此用一个砖块的集合来表示关卡
class GameLeval
{
public:
	std::vector<GameObject> mBricks;
	GameLeval() {};
	//从文件中加载关卡
	void Load(const GLchar* file, GLuint levelwidth, GLuint levelheight);
	//重新加载关卡
	void ReLoad();
	//渲染关卡
	void Draw(SpriteRender& spriterender);
	//检查一个关卡是否已经消灭,所有非坚硬砖块全被消灭
	GLboolean isCompleted();

private:
	//用砖块数据初始化关卡(砖块数据被存储在一个矩阵数据中,0表示无砖块,1表示不能被摧毁的坚固砖,大于1的表示可以被摧毁的各种砖块)
	void Init(std::vector<std::vector<GLuint>> brickMatrixData, GLuint levelwidth, GLuint levelheight);
	const GLchar* mfilepath;
	GLuint mlevelwidth,mlevelheight;
};

