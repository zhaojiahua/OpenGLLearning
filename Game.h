#pragma once
#include <glad/glad.h>

#include "GameLeval.h"
#include "BallObject.h"
#include "ScreenRect.h"
#include "PowerUp.h"

//枚举当前游戏的状态(我们可以根究当前游戏状态来决定渲染和处理不同的元素)
enum GameState
{
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};
enum Direction
{
	UP,
	RIGHT,
	DOWN,
	LEFT
};

//发生碰撞时要返回一系列的信息如发生碰撞时的运动方向矢量差等,所以我们返回一个tuple包含这些信息
typedef std::tuple<GLboolean, Direction, glm::vec2> Collision;

//一个超级游戏类,管理游戏代码,并将所有的窗口代码从游戏中解耦
class Game
{
public:
	GameState State;		//游戏状态
	GLboolean Keys[1024];	//用来处理输入
	GLuint Width, Height;		//宽高对应于玩游戏时的分辨率,用来初始化这个游戏

	//一块方片用来当做屏幕,放映渲染的画面
	ScreenRect* screenRect;
	GLuint interimBuffer, interimTex;

	std::vector<GameLeval> mLevels;//存储游戏的所有关卡
	GLuint mLevel;//当前的关卡

	std::vector<PowerUp> mPowerups;//存储游戏中的所有道具

	//构造和析构函数
	Game(GLuint width, GLuint height);
	~Game();

	void Init();//初始化游戏状态(加载所有的着色器 纹理 关卡等)
	//游戏循环(注意运动逻辑和渲染逻辑分开)
	
	//处理输入
	void ProcessInput(GLfloat dt);
	//更新游戏状态
	void Updata(GLfloat dt);
	//对游戏进行渲染
	void Render();
	//渲染画面到方片上面
	void RenderRect();

	//碰撞检测
	//AABB-AABB碰撞
	GLboolean CheckCollision(GameObject& gObj1, GameObject& gObj2);
	//AABB-圆球碰撞
	Collision CheckCollision(GameObject& gObj1, BallObject& gObj2);
	//判断碰撞时小球运动的方向(返回与target最接近的Direction)
	Direction VectorDirection(glm::vec2 target);

	//把小球和关卡中的砖块做碰撞(碰撞时要发生的事情)
	void DoCollision();

	void ResetLevel();
	void ResetPlayer();

	//生成一个过渡的中间Buffer里,并携带在相应纹理用来存储渲染结果
	void GenInterimBuffer();
	//进入循环之前shader需要设置的变量
	void SetShaderBeforeRender();

	//根据砖块的位置生成道具
	void SpawnPowerUps(GameObject& block);
	//管理当前所有激活的道具
	void UpdataPowerUps(GLfloat dt);
	//激活道具(根据道具的种类改变小球,挡板和渲染等的属性)
	void ActivePowerUp(PowerUp& inpowerup);

private:
	GLboolean ShouldSpawn(GLuint chance);
	GLboolean IsOtherPowerUpActive(std::vector<PowerUp>& powerups, std::string intype);

	//重置游戏
	void RestetGame();
};

