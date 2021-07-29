#pragma once
#include <GL/glew.h>
#include "VertexArray.h"
#include "Texture.h"
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
private:
	unsigned int renderIndex = 0;
	glm::mat4 rotationMatrix = glm::mat4(1.0f);
	bool combinedRot = false;
	static Renderer s_Instance;
	Renderer();

public:
	static Renderer& getInstance();
	Renderer(const Renderer&) = delete;
	void setRenderCombinedRot(bool rot);
	void setRenderIndex(unsigned int index);

	// draw functions
	void clear() const;
	void drawAxes(VertexArray& va, Shader& shader, glm::mat4 view, glm::mat4 projection);
	void drawLightingSource(VertexArray& va, Shader& shader, glm::mat4 view, glm::mat4 projection, glm::vec3 lightPos);
	void drawStaticObjects(VertexArray& va, Shader& shader, glm::mat4 view, glm::mat4 projection, glm::vec3 lightPos, glm::vec3 cameraPos, Texture& textureWall, Texture& textureModel, bool status, bool findDepth);
	void drawObject(VertexArray& va, Shader& shader, glm::mat4 view, glm::mat4 projection, glm::vec3 lightPos, glm::vec3 cameraPos, Texture& texture, vector<glm::mat4> modelRotMat, vector<glm::mat4> modelTransMat, float scaleFactor, glm::vec3 displacement, bool status, bool findDepth);
	void drawWall(VertexArray & va, Shader & shader, glm::mat4 view, glm::mat4 projection, glm::vec3 lightPos, glm::vec3 cameraPos, Texture& texture, vector<glm::mat4> modelRotMat, float scaleFactor, glm::vec3 displacement, bool status, bool findDepth);
	void drawMesh(VertexArray& va, Shader& shader, glm::mat4 view, glm::mat4 projection, glm::vec3 lightPos, glm::vec3 cameraPos, float scaleFactor, bool findDepth);
	void drawFloor(VertexArray& va, Shader& shader, glm::mat4 view, glm::mat4 projection, glm::vec3 lightPos, glm::vec3 cameraPos, Texture& texture, bool findDepth);
};