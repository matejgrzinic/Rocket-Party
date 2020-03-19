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
#include "Rocket.h"
#include "Block.h"

//enum Movement {
//	FORWARD,
//	BACKWARD,
//	LEFT,
//	RIGHT
//};

class Player
{
public:
	glm::vec3 m_Position;

	VertexArray* m_VA;
	IndexBuffer* m_IB;	
	std::vector<VertexBuffer*> m_Buffers;
	std::vector<Rocket*> m_Appended;
	glm::vec3 m_Color = glm::vec3(0.0f, 1.0f, 0.0f);
	float m_DistanceMoved = 20.0f;
public:
	Player();
	~Player();
	glm::vec3 GetLocation();
	void Draw(Shader& shader, Texture& texture, glm::mat4 proj, glm::mat4 view);
	void setPosition(glm::vec3 position);
	void ProcessKeyboard(glm::vec3 direction, float deltaTime, std::vector<Block*> blocks, int numOfKeysPressed);
	bool add(Rocket* r);
	void turn() {	m_DistanceMoved = 20.0f;	};
};