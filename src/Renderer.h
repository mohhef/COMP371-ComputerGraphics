#pragma once
#include<GL/glew.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

// Assertion check helper for gl commands
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) glClearError();\
    x;\
    ASSERT(glLogCall(#x, __FILE__, __LINE__))

// Empty error queue
void glClearError();
bool glLogCall(const char* function, const char* file, int line);

class Renderer
{
public:
	void clear() const;
	void draw(const VertexArray& va, const Shader& shader) const;
	// draw function should be used for Element Buffer Objects (indices)
	void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void DrawAxes(VertexArray& va, Shader& shader);

};