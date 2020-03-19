#include "Terrain.h"
#include "VertexBufferLayout.h" 

Terrain::Terrain()
	: m_X(0.0f), m_Y(0.0f), m_Z(0.0f)
{
	GLfloat positions[] = {
		-30.0f,  0.0f,  30.0f,  0.0f, 0.0f,
		 30.0f,  0.0f,  30.0f,  30.0f, 0.0f,
		 30.0f,  0.0f, -30.0f,  30.0f, 30.0f,

		 30.0f,  0.0f, -30.0f,  30.0f, 30.0f,
		-30.0f,  0.0f, -30.0f,  0.0f, 30.0f,
		-30.0f,  0.0f,  30.0f,  0.0f, 0.0f,
	};

	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 2,
		3, 4, 5,
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

Terrain::~Terrain()
{
	for (auto buffer : m_Buffers)
		delete buffer;
	delete m_VA;
	delete m_IB;
}

glm::vec3 Terrain::GetLocation()
{
	return glm::vec3(m_X, m_Y, m_Z);
}

void Terrain::Draw(Shader& shader, Texture& texture, glm::mat4 proj, glm::mat4 view)
{
	Renderer r;
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(m_X, m_Y, m_Z));
	glm::mat4 mvp = proj * view * model;
	shader.SetUniformMat4f("u_MVP", mvp);
	shader.SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
	texture.Bind();
	r.Draw(*m_VA, *m_IB, shader);
}
