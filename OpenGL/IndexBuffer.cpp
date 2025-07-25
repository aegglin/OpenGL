#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
	: indexCount(count)
{
	// this is generally the same but can vary by system (since below we're using sizeof(unsigned int), not GLuint
	ASSERT(sizeof(unsigned int) == sizeof(GLuint));

	// Send data to the GPU with this data
	GLCall(glGenBuffers(1, &rendererID)); // only want one buffer
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID)); //select which buffer and what kind (array buffer)
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));

}

IndexBuffer::~IndexBuffer()
{

}

void IndexBuffer::Bind() const
{

}

void IndexBuffer::Unbind() const
{

}