#include "SpriteRender.h"

SpriteRender::SpriteRender(Shader& shader)
{
	mShader = shader;
	InitRenderData();
}

SpriteRender::~SpriteRender()
{
	glDeleteVertexArrays(1, &mVAO);
}

void SpriteRender::DrawSprite(Texture2D& texture, glm::vec2 position, glm::vec2 size, GLfloat rotate, glm::vec3 color)
{
	//先做模型矩阵变换(先缩放再旋转最后再平移)
	mShader.Use();
	glm::mat4 m_matrix = glm::mat4(1.0f);
	
	//由于四边形的中心不在原点,我们旋转之前处理一下让它以中心为原点旋转(注意矩阵乘法是从左往右的,所以顺序正好相反)
	m_matrix = glm::translate(m_matrix, glm::vec3(position, 0.0f));
	m_matrix = glm::translate(m_matrix, glm::vec3(0.5f*size, 0.0f));
	m_matrix = glm::rotate(m_matrix, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));//绕Z轴旋转
	m_matrix = glm::scale(m_matrix, glm::vec3(size, 1.0f));
	m_matrix = glm::translate(m_matrix, glm::vec3(-0.5f, -0.5f, 0.0f));//先移动到原点
	mShader.SetMat4f("m_matrix", m_matrix);
	mShader.SetVec3f("spriteColor", color);

	glActiveTexture(GL_TEXTURE0);
	texture.Bind();

	glBindVertexArray(mVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void SpriteRender::InitRenderData()
{
	GLuint mVBO;
	//1X1的小四边形
	GLfloat vertices[] = {
		//position	//uv
		0.0f,1.0f,		0.0f,1.0f,
		1.0f,0.0f,		1.0f,0.0f,
		0.0f,0.0f,		0.0f,0.0f,

		0.0f,1.0f,		0.0f,1.0f,
		1.0f,1.0f,		1.0f,1.0f,
		1.0f,0.0f,		1.0f,0.0f
	};
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
