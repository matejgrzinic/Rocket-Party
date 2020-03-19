#include "Rocket.h"
#include "VertexBufferLayout.h" 
#include "Renderer.h"

Rocket::Rocket(glm::vec3 direction)
	: m_Position(glm::vec3(5.0f, 0.5f, 5.0f)), m_Velocity(direction)
{
	GLfloat positions[] = {
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f

	};


	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 2, 3, 4, 5,
		6, 7, 8, 9, 10, 11,
		12, 13, 14, 15, 16, 17,
		18, 19, 20, 21, 22, 23,
		24, 25, 26, 27, 28, 29,
		30, 31, 32, 33, 34, 35,

		36, 37, 38, 39, 40, 41,
		42, 43, 44, 45, 46, 47,
		48, 49, 50, 51, 52, 53,
		54, 55, 56, 57, 58, 59,
		60, 61, 62, 63, 64, 65,
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
}

Rocket::~Rocket()
{
	for (auto buffer : m_Buffers)
		delete buffer;
	delete m_VA;
	delete m_IB;	
}

glm::vec3 Rocket::GetLocation()
{
	return glm::vec3(m_Position);
}

void Rocket::Draw(Shader& shader, Texture& texture, glm::mat4 proj, glm::mat4 view, float deltaTime)
{
	Renderer r;
	
	
	glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Position);
	if (!m_Done)
	{
		//m_C += 0.0003f * deltaTime;
		m_C += 0.5f * deltaTime * deltaTime * m_Force;
	}
	model = glm::rotate(model, m_C, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.1f));

	glm::mat4 mvp = proj * view * model;
	shader.SetUniformMat4f("u_MVP", mvp);
	shader.SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
	texture.Bind();
	r.Draw(*m_VA, *m_IB, shader);
}

void Rocket::setPosition(glm::vec3 position)
{
	m_Position = position;
}

void Rocket::Update(float deltaTime)
{
	if (!m_Done)
	{		
		glm::vec3 move = m_Velocity * deltaTime;
		move *= m_Force * deltaTime; // * 10
		m_Position += move;
		m_Velocity -= glm::vec3(0.0f, 0.4f, 0.0f) * deltaTime;
	}
}

void Rocket::setForce(float force)
{
	m_Force = force * 30;	
}



