#pragma once
class VertexBuffer
{
private:
	unsigned int rendererID;
public:
	// Size is in bytes, while any variables called count are element counts
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};