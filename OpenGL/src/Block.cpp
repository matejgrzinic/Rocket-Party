#include "Block.h"
#include "VertexBufferLayout.h" 

Block::Block(glm::vec3 pos)
	: m_X(pos.x), m_Y(pos.y), m_Z(pos.z)
{
	GLfloat positions[] = {
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 0.0f,

		 0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f
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

Block::~Block()
{
	for (auto buffer : m_Buffers)
		delete buffer;
	delete m_VA;
	delete m_IB;
}

glm::vec3 Block::GetLocation()
{
	return glm::vec3(m_X, m_Y, m_Z);
}

void Block::Draw(Shader& shader, Texture& texture, glm::mat4 proj, glm::mat4 view)
{
	Renderer r;
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(m_X, m_Y, m_Z));
	model = glm::scale(model, glm::vec3(2.0f));
	glm::mat4 mvp = proj * view * model;
	shader.SetUniformMat4f("u_MVP", mvp);
	shader.SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
	texture.Bind();
	r.Draw(*m_VA, *m_IB, shader);
}
