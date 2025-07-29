#pragma once
class IndexBuffer
{
private:
	unsigned int rendererID;
	unsigned int indexCount;
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int getIndexCount() const { return indexCount; };
};