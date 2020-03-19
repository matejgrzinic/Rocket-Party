#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	GLCall(glGenBuffers(1, &m_rendererID)); // nardimo 1 buffer z idjem
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID)); // poveze objekt z idjem
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW)); // nardi "data storage" za povezan buffer, poves kok velko dato pa kero posiljas
}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_rendererID)); // deleta 1 buffer iz idja
}

void VertexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID)); // poveze objekt z idjem
}

void VertexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
