#include "VertexBuffer.h"
#include "Renderer.h"

// Create, the buffer, bind it, and specify the data structure
VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	// the buffer writes into the unsigned int's memory
	GLCall(glGenBuffers(1, &rendererID)); // only want one buffer
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID)); //select which buffer and what kind (array buffer)
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW)); // STATIC_DRAW is a hint as to how many times this will be used
	// OpenGL is a state machine so it's gradually learning more about our data
}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &rendererID));
}

void VertexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));

}

void VertexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

}
