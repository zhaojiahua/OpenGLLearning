#include "Game.h"
#include "SpriteRender.h"
#include "ResourceManager.h"
#include "Shader.h"
#include "Texture2D.h"
#include "ParticleGenerator.h"

#include <GLFW/glfw3.h>

//渲染器和挡板都是全局数据
SpriteRender* renderer;
GameObject* player_paddle;
BallObject* player_ball;
//玩家挡板的数据
const GLfloat PADDLE_VELOCITY(500.0f);
//初始化球的速度和半径
const glm::vec2 BALL_VELOCITY(100.0f, -350.0f);
const GLfloat BALL_RADIUS = 12.5f;

//500个粒子
GLuint particle_num = 500;
ParticleGenerator* particleGenerator;//粒子发射器

Game::Game(GLuint width, GLuint height) :State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{}

Game::~Game(){
	delete(renderer);
}

void Game::Init()
{
	Shader tempShader = ResourceManager::LoadShader("shaders/openGLLearning46/vertexShader_sprite.c", "shaders/openGLLearning46/fragmentShader_sprite.c", nullptr, "sprite");
	//2D投影矩阵(正交矩阵)
	glm::mat4 p_matrix = glm::ortho(0.0f, (GLfloat)Width, (GLfloat)Height, 0.0f, -1.0f, 1.0f);//底部600顶部0,这样投影结果正好和屏幕坐标系一致
	tempShader.Use();
	tempShader.SetInt("texture_diffuse", 0);
	tempShader.SetMat4f("p_matrix", p_matrix);
	renderer = new SpriteRender(tempShader);
	//加载所有的纹理和关卡
	ResourceManager::LoadTexture2D("assets/textures/face.png", GL_FALSE, "face");
	ResourceManager::LoadTexture2D("assets/Breakout_Game/background.jpg", GL_FALSE, "background");
	ResourceManager::LoadTexture2D("assets/Breakout_Game/block.png", GL_FALSE, "block");
	ResourceManager::LoadTexture2D("assets/Breakout_Game/block_solid.png", GL_FALSE, "solid");
	GameLeval level1; level1.Load("assets/Breakout_Game/level1.txt", Width, Height * 0.5);
	mLevels.push_back(level1);
	GameLeval level2; level2.Load("assets/Breakout_Game/level2.txt", Width, Height * 0.5);
	mLevels.push_back(level2);
	GameLeval level3; level3.Load("assets/Breakout_Game/level3.txt", Width, Height * 0.5);
	mLevels.push_back(level3);
	GameLeval level4; level4.Load("assets/Breakout_Game/level4.txt", Width, Height * 0.5);
	mLevels.push_back(level4);
	mLevel = 1;

	//挡板数据(按照屏幕的尺寸设置挡板的size数据)
	glm::vec2 PADDLE_SIZE(0.1 * Width, 0.025 * Height);
	ResourceManager::LoadTexture2D("assets/Breakout_Game/paddle.png", GL_FALSE, "paddle");
	glm::vec2 playerPos = glm::vec2(Width / 2 - PADDLE_SIZE.x / 2, Height - PADDLE_SIZE.y);
	Texture2D paddleTex = ResourceManager::GetTexture2D("paddle");
	player_paddle = new GameObject(playerPos, PADDLE_SIZE, paddleTex);

	//初始化球的数据
	ResourceManager::LoadTexture2D("assets/Breakout_Game/ball.png", GL_FALSE, "ball");
	glm::vec2 ballpos(playerPos.x + PADDLE_SIZE.x / 2 - BALL_RADIUS, playerPos.y - BALL_RADIUS * 2);
	Texture2D ballTex = ResourceManager::GetTexture2D("ball");
	player_ball = new BallObject(ballpos, BALL_RADIUS, BALL_VELOCITY, ballTex);

	//粒子发射器
	Shader particleShader = ResourceManager::LoadShader("shaders/openGLLearning46/vertexShader_particle.c", "shaders/openGLLearning46/fragmentShader_particle.c", nullptr, "particle");
	Texture2D particleTex= ResourceManager::LoadTexture2D("assets/Breakout_Game/particle.png", GL_FALSE, "particle");
	particleShader.Use();
	particleShader.SetInt("texture_diffuse", 0);
	particleShader.SetMat4f("p_matrix", p_matrix);
	particleShader.SetMat4f("m_matrix", glm::mat4(1.0f));
	particleGenerator = new ParticleGenerator(particleShader, particleTex, particle_num);

	//渲染方片的Shader先加载好
	Shader rectShader=ResourceManager::LoadShader("shaders/openGLLearning46/vertexShader_rect.c", "shaders/openGLLearning46/fragmentShader_rect.c", nullptr, "rect");
	screenRect = new ScreenRect(rectShader);
	screenRect->InitRenderData();

	//加载道具贴图
	ResourceManager::LoadTexture2D("assets/Breakout_Game/daoju/powerup_chaos.png", GL_FALSE, "powerup_chaos");
	ResourceManager::LoadTexture2D("assets/Breakout_Game/daoju/powerup_confuse.png", GL_FALSE, "powerup_confuse");
	ResourceManager::LoadTexture2D("assets/Breakout_Game/daoju/powerup_increase.png", GL_FALSE, "powerup_increase");
	ResourceManager::LoadTexture2D("assets/Breakout_Game/daoju/powerup_passthrough.png", GL_FALSE, "powerup_passthrough");
	ResourceManager::LoadTexture2D("assets/Breakout_Game/daoju/powerup_speed.png", GL_FALSE, "powerup_speed");
	ResourceManager::LoadTexture2D("assets/Breakout_Game/daoju/powerup_sticky.png", GL_FALSE, "powerup_sticky");
}

void Game::ProcessInput(GLfloat dt)
{
	if (State == GAME_ACTIVE) {
		GLfloat velocity = PADDLE_VELOCITY * dt;
		if (Keys[GLFW_KEY_A]) {//A按键挡板向左移动
			if (player_paddle->position.x > 0) player_paddle->position.x -= velocity;
		}
		if (Keys[GLFW_KEY_D]) {//D按键挡板向右移动
			if (player_paddle->position.x < Width - player_paddle->size.x) player_paddle->position.x += velocity;
		}
		if (Keys[GLFW_KEY_SPACE]) {//空格键发射小球
			player_ball->mStuck = GL_FALSE;
		}
	}
}

void Game::Updata(GLfloat dt)
{
	//实时更新球的位置
	if (player_ball->mStuck) player_ball->position.x = player_paddle->position.x + player_paddle->size.x / 2.0f - player_ball->mRadius;
	player_ball->Move(dt, Width);
	DoCollision();
	if (screenRect->mShakeTime > 0) {
		screenRect->mShakeTime -= dt;
		screenRect->mShake = GL_TRUE;
	}
	else screenRect->mShake = GL_FALSE;

	//实时更新道具的位置
	for (PowerUp& power : mPowerups) {
		if (!power.bDestroyed) {
			power.position.y += abs(power.velocity.y) * dt;
			power.position.x += power.velocity.x * dt;
		}
	}
	UpdataPowerUps(dt);
	//更新粒子
	particleGenerator->UpdataParticle(dt, 2, *player_ball, glm::vec2(player_ball->mRadius / 2.0f));

	//如果球触碰到了底部边界
	if (player_ball->position.y >= Height) {
		RestetGame();
	}
}

void Game::Render()
{
	if (State == GAME_ACTIVE) {
		//绘制背景
		Texture2D backgroundTex = ResourceManager::GetTexture2D("background");
		renderer->DrawSprite(backgroundTex, glm::vec2(0), glm::vec2(Width, Height), 0.0f);
		//绘制关卡
		mLevels[mLevel-1].Draw(*renderer);
		//渲染挡板
		player_paddle->Draw(*renderer);
		//渲染粒子(在球的后面)
		particleGenerator->Draw();
		//绘制球
		player_ball->Draw(*renderer);
		//绘制道具
		for (PowerUp& power : mPowerups) {
			if (!power.bDestroyed)power.Draw(*renderer);
		}
	}	
}

void Game::RenderRect()
{
	screenRect->mTex = interimTex;
	screenRect->Draw();
}

GLboolean Game::CheckCollision(GameObject& gObj1, GameObject& gObj2)
{
	//AABB检测算法
	//x轴向检测
	GLboolean xb = gObj1.position.x + gObj1.size.x >= gObj2.position.x && gObj2.position.x + gObj2.size.x >= gObj1.position.x;
	//y轴向检测
	GLboolean yb = gObj1.position.y + gObj1.size.y >= gObj2.position.y && gObj2.position.y + gObj2.size.y >= gObj1.position.y;
	return xb && yb;
}

Collision Game::CheckCollision(GameObject& gObj1, BallObject& gObj2)
{
	//圆心
	glm::vec2 ball_center(gObj2.position + gObj2.mRadius);
	//AABB
	glm::vec2 half_extents(gObj1.size * 0.5f);
	glm::vec2 aabb_center(gObj1.position + half_extents);
	//中心差值矢量
	glm::vec2 diff = ball_center - aabb_center;//这里要注意向量方向
	//限制差值矢量求得最近点
	glm::vec2 clamped = glm::clamp(diff, -half_extents, half_extents);
	glm::vec2 closest = aabb_center + clamped;
	//然后判断最近点到圆心的距离是否大于圆半径
	if (glm::length(closest - ball_center) <= gObj2.mRadius) {
		return make_tuple(GL_TRUE, VectorDirection(diff), diff);
	}
	else return make_tuple(GL_FALSE, UP, glm::vec2(0.0f));
}

Direction Game::VectorDirection(glm::vec2 target)
{
	glm::vec2 compass[] = {
		glm::vec2(0.0f,1.0f),
		glm::vec2(1.0f,0.0f),
		glm::vec2(0.0f,-1.0f),
		glm::vec2(-1.0f,0.0f)
	};
	GLfloat max = 0.0f;
	GLuint best_match = -1;
	for (GLuint i = 0; i < 4; i++) {
		GLfloat dot_product = glm::dot(glm::normalize(target), compass[i]);
		if (dot_product > max) {
			max = dot_product;
			best_match = i;
		}
	}
	return (Direction)best_match;
}

void Game::DoCollision()
{
	//遍历所有的砖块检测与小球的碰撞情况
	for (GameObject& brick : mLevels[mLevel-1].mBricks) {
		if (!brick.bDestroyed) {
			Collision collisionInfo = CheckCollision(brick, *player_ball);
			if (get<0>(collisionInfo)) {//如果检测到碰撞
				//cout << "collision" << endl;
				if (!brick.isSolid) brick.bDestroyed = GL_TRUE;
				SpawnPowerUps(brick);//随机生成道具
				//碰撞处理
				screenRect->mShakeTime = 0.1f;
				Direction dir = get<1>(collisionInfo);
				glm::vec2 diff_vec = get<2>(collisionInfo);
				if (!(player_ball->mPassThrough && !brick.isSolid)) {
					if (dir == LEFT || dir == RIGHT) {
						player_ball->velocity.x *= -1;//改变X方向
						//重定位
						GLfloat penetration = player_ball->mRadius - abs(diff_vec.x);
						if (dir == LEFT) player_ball->position.x += penetration;
						else player_ball->position.x -= penetration;
					}
					else {
						player_ball->velocity.y *= -1;//改变Y方向
						//重定位
						GLfloat penetration = player_ball->mRadius - abs(diff_vec.y);
						if (dir == DOWN) player_ball->position.y += penetration;
						else player_ball->position.y -= penetration;
					}
				}
				
			}
		}
	}

	//挡板和碰撞球
	Collision player_collisionInfo = CheckCollision(*player_paddle, *player_ball);
	if (!player_ball->mStuck && get<0>(player_collisionInfo)) {
		//检测碰到挡板的位置(根据这个来改变小球的速度)
		GLfloat paddle_center = player_paddle->position.x + player_paddle->size.x / 2.0f;
		GLfloat distance = player_ball->position.x + player_ball->mRadius - paddle_center;
		GLfloat percentage = distance / (player_paddle->size.x / 2.0f);
		//改变小球
		GLfloat stength = 2.0f;
		glm::vec2 tempVelocity = player_ball->velocity;
		tempVelocity.x = BALL_VELOCITY.x * stength * percentage;
		tempVelocity.y = -1 * abs(player_ball->velocity.y);
		player_ball->velocity = glm::normalize(tempVelocity) * glm::length(player_ball->velocity);
		player_ball->mStuck = player_ball->mSticky;
	}

	//道具和挡板之间的碰撞
	for (PowerUp& powerup : mPowerups) {
		if (!powerup.bDestroyed) {
			if (powerup.position.y >= Height)powerup.bDestroyed = GL_TRUE;
			if (CheckCollision(*player_paddle, powerup)) {
				ActivePowerUp(powerup);
				powerup.bDestroyed = GL_TRUE;
				powerup.mActivated = GL_TRUE;
			}
		}
	}
}

void Game::ResetLevel()
{
	mLevels[mLevel].ReLoad();
}

void Game::ResetPlayer()
{
	glm::vec2 PADDLE_SIZE(0.1 * Width, 0.025 * Height);
	player_paddle->position= glm::vec2(Width / 2 - PADDLE_SIZE.x / 2, Height - PADDLE_SIZE.y);
	player_ball->Reset(player_paddle->position + glm::vec2(player_paddle->size.x / 2.0f - BALL_RADIUS, -(BALL_RADIUS * 2.0f)), BALL_VELOCITY);
}

void Game::GenInterimBuffer()
{
	glGenFramebuffers(1, &interimBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, interimBuffer);

	glGenTextures(1, &interimTex);
	glBindTexture(GL_TEXTURE_2D, interimTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, interimTex, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)cout << "preBuffer not complete" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


}

void Game::SetShaderBeforeRender()
{
	//配置RectShader的卷积核
	GLfloat offset = 1.0f / 300.0f;
	GLfloat offsets[9][2] = {
		{-offset,offset},{0.0f,offset}, {offset,offset},
		{-offset,0.0f},	{0.0f,0.0f},		 {offset,0.0},
		{-offset,-offset},{0.0f,-offset},{offset,-offset}
	};
	GLint edge_kernel[9] = {
			-1, -1, -1,
			-1,  8, -1,
			-1, -1, -1
	};
	GLfloat blure_kernel[9] = {
		1.0 / 16, 2.0 / 16, 1.0 / 16,
		2.0 / 16, 4.0 / 16, 2.0 / 16,
		1.0 / 16, 2.0 / 16, 1.0 / 16
	};
	Shader tempRectShader = ResourceManager::GetShader("rect");
	tempRectShader.Use();
	tempRectShader.SetInt("texture_diffuse", 0);
	glUniform2fv(glGetUniformLocation(tempRectShader.ID, "offsets"), 9, (GLfloat*)offsets);
	glUniform1iv(glGetUniformLocation(tempRectShader.ID, "edge_kernel"), 9, edge_kernel);
	glUniform1fv(glGetUniformLocation(tempRectShader.ID, "blur_kernel"), 9, blure_kernel);
}

void Game::SpawnPowerUps(GameObject& block)
{
	if (ShouldSpawn(10)) {//	1/50的概率
		mPowerups.push_back(PowerUp("speed", glm::vec3(0.5f, 0.5f, 1.0f), 4.0f, block.position, ResourceManager::GetTexture2D("powerup_speed")));
	}
	if (ShouldSpawn(50)) {
		mPowerups.push_back(PowerUp("sticky", glm::vec3(1.0f, 0.5f, 1.0f), 6.0f, block.position, ResourceManager::GetTexture2D("powerup_sticky")));
	}
	if (ShouldSpawn(50)) {
		mPowerups.push_back(PowerUp("passthrough", glm::vec3(1.0f, 0.5f, 1.0f), 4.0f, block.position, ResourceManager::GetTexture2D("powerup_passthrough")));
	}
	if (ShouldSpawn(50)) {
		mPowerups.push_back(PowerUp("pad_size_increase", glm::vec3(1.0f, 0.5f, 1.0f), 6.0f, block.position, ResourceManager::GetTexture2D("powerup_increase")));
	}
	if (ShouldSpawn(10)) {
		mPowerups.push_back(PowerUp("confuse", glm::vec3(1.0f, 0.5f, 1.0f), 4.0f, block.position, ResourceManager::GetTexture2D("powerup_confuse")));
	}
	if (ShouldSpawn(10)) {
		mPowerups.push_back(PowerUp("choas", glm::vec3(1.0f, 0.5f, 1.0f), 5.0f, block.position, ResourceManager::GetTexture2D("powerup_chaos")));
	}
}

void Game::UpdataPowerUps(GLfloat dt)
{
	for (PowerUp& powerup : mPowerups) {
		if (powerup.mActivated) {
			powerup.mDuration -= dt;
			//cout << powerup.mDuration << endl;
			if (powerup.mDuration <= 0.0f) {//过时的道具消除效果
				powerup.mActivated = GL_FALSE;
				if (powerup.mType == "speed") {
					if (!IsOtherPowerUpActive(mPowerups, "speed")) {//如果其他的道具也没有激活speed的时候,重置此效果状态
						player_ball->velocity *= 0.5f;
					}
				}
				else if (powerup.mType == "sticky") {
					if (!IsOtherPowerUpActive(mPowerups, "sticky")) {//如果其他的道具也没有激活sticky的时候,重置此效果状态
						player_ball->mSticky = GL_FALSE;
						player_paddle->color = glm::vec3(1.0f);
					}
				}
				else if (powerup.mType == "passthrough") {
					if (!IsOtherPowerUpActive(mPowerups, "passthrough")) {//如果其他的道具也没有激活sticky的时候,重置此效果状态
						player_ball->mPassThrough = GL_FALSE;
						player_ball->color = glm::vec3(1.0f);
					}
				}
				else if (powerup.mType == "pad_size_increase") {
					if (!IsOtherPowerUpActive(mPowerups, "pad_size_increase")) {//如果其他的道具也没有激活sticky的时候,重置此效果状态
						player_paddle->size.x -= 50.0f;
					}
				}
				else if (powerup.mType == "confuse") {
					if (!IsOtherPowerUpActive(mPowerups, "confuse")) {//如果其他的道具也没有激活sticky的时候,重置此效果状态
						screenRect->mConfuse = GL_FALSE;
					}
				}
				else if (powerup.mType == "choas") {
					if (!IsOtherPowerUpActive(mPowerups, "choas")) {//如果其他的道具也没有激活sticky的时候,重置此效果状态
						screenRect->mChos = GL_FALSE;
					}
				}
			}
		}
	}
	//如果检测到一个道具被销毁或者被停用,就将这个道具放在容器末尾并移除
	mPowerups.erase(remove_if(mPowerups.begin(), mPowerups.end(), [](const PowerUp& tempPower) {return tempPower.bDestroyed && !tempPower.mActivated; }), mPowerups.end());
}

void Game::ActivePowerUp(PowerUp& inpowerup)
{
	if (inpowerup.mType == "speed") player_ball->velocity *= 2.0f;
	else if (inpowerup.mType == "sticky") {
		player_ball->mSticky = GL_TRUE;
		player_paddle->color = glm::vec3(1.0f, 0.5f, 1.0f);
	}
	else if (inpowerup.mType == "passthrough") {
		player_ball->mPassThrough = GL_TRUE;
		player_ball->color = glm::vec3(1.0f, 0.5f, 0.5f);
	}
	else if (inpowerup.mType == "pad_size_increase")player_paddle->size.x += 50;
	else if (inpowerup.mType == "confuse") {
		if (!screenRect->mChos)screenRect->mConfuse = GL_TRUE;//只有在choas未激活时才能生效
	}
	else if (inpowerup.mType == "choas") {
		if (!screenRect->mConfuse)screenRect->mChos = GL_TRUE;//只有在mConfuse未激活时才能生效
	}
}

GLboolean Game::ShouldSpawn(GLuint chance)
{
	GLuint randResult = rand() % chance;
	return randResult == 0;
}

GLboolean Game::IsOtherPowerUpActive(std::vector<PowerUp>& powerups, std::string intype)
{
	for (const PowerUp& power : mPowerups) {
		if (power.mActivated)
			if (power.mType == intype)return GL_TRUE;
	}
	return GL_FALSE;
}

void Game::RestetGame()
{
	ResetLevel();
	ResetPlayer();
	mPowerups.clear();//清空所有道具
	//重置画布
	screenRect->mChos = GL_FALSE;
	screenRect->mShake = GL_FALSE;
	screenRect->mConfuse = GL_FALSE;
	
}



