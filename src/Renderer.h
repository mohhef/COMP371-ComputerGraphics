#pragma once
#include <GL/glew.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Constants.h"

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
private:
	unsigned int renderMethod = GL_TRIANGLES;
	static Renderer s_Instance;
	Renderer();
public:
	static Renderer& getInstance();
	Renderer(const Renderer&) = delete;
	void setRenderMethod(unsigned int method);
	void clear() const;
	void draw(const VertexArray& va, const Shader& shader) const;

	// draw function should be used for Element Buffer Objects (indices)
	void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void drawAxes(VertexArray& va, Shader& shader, glm::mat4 view, glm::mat4 projection);
	void drawObject(VertexArray& va, Shader& shader, vector<glm::mat4> moddelRotMat, vector<glm::mat4> modelTransMat, float scaleFactor, glm::vec3 displacement);
	void drawWall(VertexArray & va, Shader & shader, float scaleFactor, glm::vec3 displacement);
	void drawMesh(VertexArray& va, Shader& shader, glm::mat4 view, glm::mat4 projection, float scaleFactor);
};