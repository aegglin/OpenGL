#pragma once

#include <vector>

struct VertexBufferElement
{
	unsigned int count;
	unsigned int type;
	bool isNormalized;
};

class VertexBufferLayout {
private:

	std::vector<VertexBufferElement> elements;
public:
	VertexBufferLayout();

	template<typename T>
	void Push(int count)
	{
		static_assert(false); // if type is unmatched
	}
	//template specializations
	template<>
	void Push<float>(int count, bool isNormalizedg)
	{
		elements.push_back();
	}
};