#pragma once

#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "VertexBuffer.h"
#include <vector>
#include "Texture.h"

class Block
{
public:
	float m_X;
	float m_Y;
	float m_Z;

	VertexArray* m_VA;
	IndexBuffer* m_IB;
	std::vector<VertexBuffer*> m_Buffers;
public:
	Block(glm::vec3 pos);
	~Block();
	glm::vec3 GetLocation();
	void Draw(Shader& shader, Texture& texture, glm::mat4 proj, glm::mat4 view);
};