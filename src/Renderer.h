#pragma once
#include <GL/glew.h>
#include "VertexArray.h"
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
	unsigned int renderIndex = 0;
	glm::mat4 rotationMatrix = glm::mat4(1.0f);
	bool combinedRot = false;
	static Renderer s_Instance;
	Renderer();

public:
	static Renderer& getInstance();
	Renderer(const Renderer&) = delete;
	void setRenderMethod(unsigned int method);
	void setRenderCombinedRot(bool rot);
	void setRenderIndex(unsigned int index);

	// draw functions
	void clear() const;
	void drawAxes(VertexArray& va, Shader& shader, glm::mat4 view, glm::mat4 projection);
	void drawLightingSource(VertexArray& va, Shader& shader, glm::vec3 lightPos, glm::mat4 view, glm::mat4 projection);
	void drawStaticObjects(VertexArray& va, Shader& shader, glm::mat4 view, glm::mat4 projection, glm::vec3 lightPos, glm::vec3 cameraPos);
	void drawObject(VertexArray& va, Shader& shader, vector<glm::mat4> moddelRotMat, vector<glm::mat4> modelTransMat, float scaleFactor, glm::vec3 displacement, glm::mat4 view, glm::mat4 projection, glm::vec3 lightPos, glm::vec3 cameraPos);
	void drawWall(VertexArray & va, Shader & shader, vector<glm::mat4> modelRotMat, float scaleFactor, glm::vec3 displacement, glm::mat4 view, glm::mat4 projection, glm::vec3 lightPos, glm::vec3 cameraPos, unsigned int textureID);
	void drawMesh(VertexArray& va, Shader& shader, glm::mat4 view, glm::mat4 projection, float scaleFactor);

	void drawTexture(VertexArray& va, Shader& shader, glm::mat4 view, glm::mat4 projection, glm::vec3 lightPos, glm::vec3 cameraPos, unsigned int textureID);
};