#pragma once
#include <iostream>
using namespace std;

class Texture {
public:
	Texture(const std::string& path);
	~Texture();
	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int getWidth() const { return width; }
	inline int getHeight() const { return height; }
private:
	unsigned int id; // texture id
	string filePath; // file path to texture
	unsigned char* localBuffer; // holds all data
	int width, height, nrChannels; 
};