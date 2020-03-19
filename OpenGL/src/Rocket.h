#pragma once

#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "VertexBuffer.h"
#include <vector>
#include "Texture.h"
#include "Camera.h"


class Rocket
{
public:
	glm::vec3 m_Position;
	glm::vec3 m_Velocity = glm::vec3(1.0f);
	float m_C = 1;
	float m_Force;

	VertexArray* m_VA;
	IndexBuffer* m_IB;
	std::vector<VertexBuffer*> m_Buffers;
public:
	Rocket(glm::vec3 direction);
	~Rocket();
	glm::vec3 GetLocation();
	void Draw(Shader& shader, Texture& texture, glm::mat4 proj, glm::mat4 view, float deltaTime);
	void setPosition(glm::vec3 position);
	void Update(float deltaTime);
	void setForce(float force);
	bool m_Done = false;	
};