#include "VertexArray.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray()
{
	// when you specify the vertex attrib pointer, it binds the vertex array to the data (in fewer steps)
	GLCall(glGenVertexArrays(1, &rendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &rendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind(); // bind vertex array
	vb.Bind(); // bind buffer
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		// Tell OpenGL what kind/how much data we're giving it
		GLCall(glEnableVertexAttribArray(i)); //order of enabling and having the pointer doesn't matter because it's a state machine (as long as buffer bound)
		GLCall(glVertexAttribPointer(i, element.count, element.type,
			element.isNormalized, layout.GetStride(), (const void*)offset));// only specifying one attribute so only need to call this once
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
	
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(rendererID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}
