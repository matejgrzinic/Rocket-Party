#include "Player.h"
#include "VertexBufferLayout.h" 
#include "Renderer.h"

Player::Player()
	: m_Position(glm::vec3(0.0f, 0.5f, 0.0f))
{
	GLfloat positions[] = {
		-0.3f,	-0.5f,  -0.25f, 0.0f, 0.0f,
		 0.3f,	-0.5f,  -0.25f, 1.0f, 0.0f,
		 0.3f,	 0.5f,  -0.25f, 1.0f, 1.0f,
		 0.3f,	 0.5f,  -0.25f, 1.0f, 1.0f,
		-0.3f,	 0.5f,  -0.25f, 0.0f, 1.0f,
		-0.3f,	-0.5f,  -0.25f, 0.0f, 0.0f,

		-0.3f,	-0.5f,  0.25f, 0.0f, 0.0f,
		 0.3f,	-0.5f,  0.25f, 1.0f, 0.0f,
		 0.3f,	 0.5f,  0.25f, 1.0f, 1.0f,
		 0.3f,	 0.5f,  0.25f, 1.0f, 1.0f,
		-0.3f,	 0.5f,  0.25f, 0.0f, 1.0f,
		-0.3f,	-0.5f,  0.25f, 0.0f, 0.0f,

		-0.3f,	 0.5f,  0.25f, 0.0f, 0.0f,
		-0.3f,	 0.5f,  -0.25f, 1.0f, 0.0f,
		-0.3f,	-0.5f,  -0.25f, 1.0f, 1.0f,
		-0.3f,	-0.5f,  -0.25f, 1.0f, 1.0f,
		-0.3f,	-0.5f,  0.25f, 0.0f, 1.0f,
		-0.3f,	 0.5f,  0.25f, 0.0f, 0.0f,

		 0.3f,	 0.5f,  0.25f, 0.0f, 0.0f,
		 0.3f,	 0.5f, -0.25f, 1.0f, 0.0f,
		 0.3f,	-0.5f, -0.25f, 1.0f, 1.0f,
		 0.3f,	-0.5f, -0.25f, 1.0f, 1.0f,
		 0.3f,	-0.5f,  0.25f, 0.0f, 1.0f,
		 0.3f,	 0.5f,  0.25f, 0.0f, 0.0f,

		-0.3f,	-0.5f, -0.25f, 0.0f, 1.0f,
		 0.3f,	-0.5f, -0.25f, 1.0f, 1.0f,
		 0.3f,	-0.5f,  0.25f, 1.0f, 0.0f,
		 0.3f,	-0.5f,  0.25f, 1.0f, 0.0f,
		-0.3f,	-0.5f,  0.25f, 0.0f, 0.0f,
		-0.3f,	-0.5f,  -0.25f, 0.0f, 1.0f,

		-0.3f,	 0.5f, -0.25f, 0.0f, 1.0f,
		 0.3f,	 0.5f, -0.25f, 1.0f, 1.0f,
		 0.3f,	 0.5f,  0.25f, 1.0f, 0.0f,
		 0.3f,	 0.5f,  0.25f, 1.0f, 0.0f,
		-0.3f,	 0.5f,  0.25f, 0.0f, 0.0f,
		-0.3f,	 0.5f, -0.25f, 0.0f, 1.0f


	};	
	

	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 2, 3, 4, 5,
		6, 7, 8, 9, 10, 11,
		12, 13, 14, 15, 16, 17,
		18, 19, 20, 21, 22, 23,
		24, 25, 26, 27, 28, 29,
		30, 31, 32, 33, 34, 35,		
	};

	m_VA = new VertexArray();
	m_VA->Bind();

	VertexBuffer* vb = new VertexBuffer(positions, sizeof(positions));
	m_Buffers.push_back(vb);
	m_Buffers.push_back(new VertexBuffer(positions, sizeof(positions)));

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(2);
	m_VA->AddBuffer(*vb, layout);



	m_IB = new IndexBuffer(indices, sizeof(indices) / sizeof(GLuint));	
	//m_Shader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
}

Player::~Player()
{
	for (auto buffer : m_Buffers)
		delete buffer;
	delete m_VA;
	delete m_IB;
}

glm::vec3 Player::GetLocation()
{
	return glm::vec3(m_Position);
}

void Player::Draw(Shader& shader, Texture& texture, glm::mat4 proj, glm::mat4 view)
{
	Renderer r;
	/*glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f));
	model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));*/

	glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Position);

	glm::mat4 mvp = proj * view * model;
	shader.SetUniformMat4f("u_MVP", mvp);
	shader.SetUniform4f("u_Color", m_Color.x, m_Color.y, m_Color.z, 1.0f);
	texture.Bind();
	r.Draw(*m_VA, *m_IB, shader);
}

void Player::setPosition(glm::vec3 position)
{
	m_Position = position;
}

void Player::ProcessKeyboard(glm::vec3 direction, float deltaTime, std::vector<Block*> blocks, int numOfKeysPressed)
{
	//std::cout << m_DistanceMoved << std::endl;
	if (m_DistanceMoved > 0)
		{
		float m_MovementSpeed = (numOfKeysPressed > 0) ? 5.0f / numOfKeysPressed : 0.0f;
		float velocity = m_MovementSpeed * deltaTime;

		direction = glm::normalize(glm::vec3(direction.x, 0.0f, direction.z)) * velocity;
		
		

		glm::vec3 posDiff = m_Position;
		glm::vec3 positionMoved = m_Position + direction;

		
		if (positionMoved.x < -29.73f || positionMoved.x > 29.73f)
		{
			if (positionMoved.z < -29.78f || positionMoved.z > 29.78f)
				return;

			positionMoved.x = m_Position.x;
			
		}
		else if (positionMoved.z < -29.78f || positionMoved.z > 29.78f)
		{
			positionMoved.z = m_Position.z;			
		}		

		for (Block* b : blocks) {
			float distx = pow(positionMoved.x - b->GetLocation().x, 2);
			float distz = pow(positionMoved.z - b->GetLocation().z, 2);

			if (distx < 1.7 && distz < 1.57)
			{			
				
				if (distz + 0.13 < distx)
					positionMoved.x = m_Position.x;
				else
					positionMoved.z = m_Position.z;				
				break;
			}				
		}

		
		m_Position = positionMoved;
		posDiff -= m_Position;
		posDiff *= -1;

		float traveled = posDiff.x * posDiff.x + posDiff.y * posDiff.y + posDiff.z * posDiff.z;
		traveled = sqrt(traveled);

		m_DistanceMoved -= traveled;

		for (Rocket* r : m_Appended)
		{
			r->setPosition(r->GetLocation() + posDiff);
		}
		
	}
}

bool Player::add(Rocket* r)
{
	m_Appended.push_back(r);
	
	switch (m_Appended.size())
	{
		case 1:	m_Color = glm::vec3(1.0f, 1.0f, 0.0f);
			break;
		case 2: m_Color = glm::vec3(1.0f, 0.0f, 0.0f);
			break;
		default:
			return true;
	}
	return false;
}
