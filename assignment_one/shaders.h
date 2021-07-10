#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

using namespace std;

class Shader 
{
public:

	// program id
	unsigned int id;

	Shader(string vertexPath, string fragmentPath);

	void activate();

	void setVec4(string name, float x, float y, float z, float w) const;
	void setVec4(string name, glm::vec4 vec) const;
	void setMat4(string name, glm::mat4 mat) const;
};