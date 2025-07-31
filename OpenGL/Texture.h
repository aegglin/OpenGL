#pragma once

#include "Renderer.h"

class Texture
{
public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

private:
	unsigned int rendererID;
	std::string filePath;
	int width, height, bpp;
	unsigned char* localBuffer;

	

	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }
};