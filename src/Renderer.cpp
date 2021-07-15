#include "Renderer.h"
#include <iostream>
#include "Shader.h"

void glClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool glLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL Error] (" << error << ")" << function << " " <<
			" " << file << " " << line << std::endl;
		return false;
	}
	return true;
}

void Renderer::clear() const
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

Renderer Renderer::s_Instance;
Renderer::Renderer() {};

Renderer& Renderer::getInstance()
{
	return s_Instance;
}

void Renderer::setRenderMethod(unsigned int method) 
{
	renderMethod = method;
}

void Renderer::drawAxes(VertexArray& va, Shader& shader, glm::mat4 view, glm::mat4 projection)
{
	va.bind();
	shader.bind();

	glm::mat4 model = glm::mat4(1.0f);

	shader.setUniform4Mat("view", view);
	shader.setUniform4Mat("projection", projection);
	shader.setUniform4Mat("model", model);

	shader.setUniform4Vec("ourColor", glm::vec4(1.0, 0.0, 0.0, 1.0));
	shader.setUniform4Mat("model", model);
	GLCall(glDrawArrays(GL_LINES, 0, 2));

	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setUniform4Vec("ourColor", glm::vec4(0.0, 1.0, 0.0, 1.0));
	shader.setUniform4Mat("model", model);
	GLCall(glDrawArrays(GL_LINES, 0, 2));


	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setUniform4Mat("model", model);
	shader.setUniform4Vec("ourColor", glm::vec4(0.0, 0.0, 1.0, 1.0));
	GLCall(glDrawArrays(GL_LINES, 0, 2));

	va.unbind();
	shader.unbind();
}

void Renderer::drawObject(VertexArray& va, Shader& shader, vector<glm::mat4> modelRotMat, vector<glm::mat4> modelTransMat, float scaleFactor, glm::vec3 displacement) {
	va.bind();
	shader.bind();

	shader.setUniform4Vec("ourColor", glm::vec4(0, 1, 1, 1));
	int numCubePieces = modelCubePositions.at(modelIndex).size();
	float time = (float)glfwGetTime();
	for (int i = 0; i < numCubePieces; i++)
	{
		glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(displacement.x, 0.0f, displacement.z));
		glm::mat4 initialPos = glm::translate(glm::mat4(1.0f), modelPosition.at(modelIndex));

		// unit matrix * scaling input * xz_translation * model_translation (align with hole) * model_rotation * model_cube_scale * model_cube_translation
		glm::mat4 model = glm::mat4(1.0f) * glm::scale(glm::mat4(1.0f), glm::vec3(scaleFactor)) * trans * initialPos * modelRotMat.at(i) * modelScale.at(modelIndex) * modelTransMat.at(i);

		shader.setUniform4Mat("model", model);
		glDrawArrays(renderMethod, 0, 36);
	}
	va.unbind();
	shader.unbind();
}

void Renderer::drawWall(VertexArray& va, Shader& shader, float scaleFactor, glm::vec3 displacement) {
	va.bind();
	shader.bind();

	shader.setUniform4Vec("ourColor", glm::vec4(0.63f, 0.63f, 0.63f, 1));
	int numWallPieces = wallCubePositions.at(modelIndex).size();
	for (int i = 0; i < numWallPieces; i++)
	{
		// calculate model matrix for each object and pass it to shader before drawing
		glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(displacement.x, 0.0f, 0.0f));
		glm::mat4 initialPos = glm::translate(glm::mat4(1.0f), wallPosition.at(modelIndex));

		// unit matrix * wall_scale * x_translation * wall_translation (align with XZ plane) * wall_cube_scale * wall_cube_translate
		glm::mat4 model = glm::mat4(1.0f)
			* glm::scale(glm::mat4(1.0f), glm::vec3(scaleFactor))
			* trans
			* initialPos
			* glm::translate(glm::mat4(1.0f), wallCubePositions.at(modelIndex).at(i))
			* glm::scale(glm::mat4(1.0f), wallScales.at(modelIndex).at(i));

		shader.setUniform4Mat("model", model);

		glDrawArrays(renderMethod, 0, 36);
	}
	va.unbind();
	shader.unbind();
}

