#include "GameLeval.h"
#include "ResourceManager.h"

void GameLeval::Load(const GLchar* file, GLuint levelwidth, GLuint levelheight)
{
	mfilepath = file;
	//首先清空过期的数据
	mBricks.clear();
	GLuint brickCode;
	GameLeval gamelevel;
	//读取关卡中的数据(按行读取)
	std::string line;
	std::ifstream mfstream(file);//读文件
	std::vector<std::vector<GLuint>> brickDatas;//用来存储数据
	if (mfstream) {//如果读取成功
		while (std::getline(mfstream,line))//按行读取
		{
			std::istringstream sstream(line);//从string里面读取字符
			std::vector<GLuint>row;//用来存放每一行数据
			while (sstream >> brickCode) row.push_back(brickCode);
			brickDatas.push_back(row);
		}
		if (brickDatas.size() > 0) Init(brickDatas, levelwidth, levelheight);
	}
}

void GameLeval::Draw(SpriteRender& spriterender)
{
	for (auto& item : mBricks) {
		if (!item.bDestroyed) {
			item.Draw(spriterender);
		}
	}
}

GLboolean GameLeval::isCompleted()
{
	for (auto& item : mBricks) {
		if (!item.isSolid && !item.bDestroyed) return GL_FALSE;
		return GL_TRUE;
	}
}

void GameLeval::Init(std::vector<std::vector<GLuint>> brickMatrixData, GLuint levelwidth, GLuint levelheight)
{
	GLuint levelHeight = brickMatrixData.size();//方块矩阵高度上的数量
	GLuint levelWidth = brickMatrixData[0].size();//方块矩阵宽度上的数量
	GLfloat unit_width = levelwidth / static_cast<GLfloat>(levelWidth);//每块砖的单位尺寸
	GLfloat unit_height = levelheight / static_cast<GLfloat>(levelHeight);//每块砖的单位尺寸
	for (GLuint y = 0; y < levelHeight; y++) {
		for (GLuint x = 0; x < levelWidth; x++) {
			glm::vec2 pos(unit_width * x, unit_height * y);
			glm::vec2 size(unit_width, unit_height);
			if (brickMatrixData[y][x] == 1) {
				GameObject obj(pos, size, ResourceManager::GetTexture2D("solid"), glm::vec3(0.8f, 0.8f, 0.7f));
				obj.isSolid = GL_TRUE;
				mBricks.push_back(obj);
			}
			else if (brickMatrixData[y][x] > 1) {
				glm::vec3 defaultcolor = glm::vec3(1.0f);//砖块默认为白色
				/*switch (brickMatrixData[y][x])
				{
				case 2:
					defaultcolor = glm::vec3(0.2f, 0.6f, 0.8f);
					break;
				case 3:
					defaultcolor = glm::vec3(0.1f, 0.8f, 0.7f);
					break;
				case 4:
					defaultcolor = glm::vec3(0.7f, 0.5f, 0.2f);
					break;
				case 5:
					defaultcolor = glm::vec3(0.7f, 0.6f, 0.8f);
					break;
				default:
					break;
				}*/
				if (brickMatrixData[y][x] == 2)defaultcolor = glm::vec3(0.2f, 0.6f, 0.8f);
				else if (brickMatrixData[y][x] == 3)defaultcolor = glm::vec3(0.1f, 0.8f, 0.7f);
				else if (brickMatrixData[y][x] == 4)defaultcolor = glm::vec3(0.7f, 0.5f, 0.2f);
				else if (brickMatrixData[y][x] == 5)defaultcolor = glm::vec3(0.7f, 0.6f, 0.8f);

				GameObject obj(pos, size, ResourceManager::GetTexture2D("block"), defaultcolor);
				mBricks.push_back(obj);
			}
		}
	}
}

void GameLeval::ReLoad()
{
	Load(mfilepath, mlevelwidth, mlevelheight);
}
