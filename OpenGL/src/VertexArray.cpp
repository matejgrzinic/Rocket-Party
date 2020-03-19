#include "VertexArray.h"
#include "Renderer.h"
#include <iostream>
#include <string>
#include "VertexBufferLayout.h"

VertexArray::VertexArray()
{
	// nardi vertex array objekte. vertex arrray hrani format vertex date in buffer objektov
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	// 
	Bind();
	vb.Bind(); 
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;	
	for (unsigned int i = 0; i < elements.size(); i++)
	{		
		const auto& element = elements[i];
		// v vertex shaderju pod kerim layoutom je
		GLCall(glEnableVertexAttribArray(i));
		// Ker index, iz kok komponent je sestavlen, ce naj opengl normalizera, kasn je offset med isto komponento, pa kasn je offset za naso komponento od starta (const void*)8 
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
	
}

void VertexArray::Bind() const
{
	// poveze vertex array z nekim idjem
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}
