#pragma once
#include "Renderer.h"

class Texture {
public:
	Texture(const std::string& path);
	~Texture();
	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int getWidth() const { return width; }
	inline int getHeight() const { return height; }
private:
	unsigned int id;
	std::string filePath;
	unsigned char* localBuffer;
	int width, height, nrChannels;
};