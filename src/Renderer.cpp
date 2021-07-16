#include "Renderer.h"
#include <iostream>
#include "Shader.h"

void glClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool glLogCall(const char* function, const char* file, int line) 
{
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

void Renderer::setRenderIndex(unsigned int index)
{
	renderIndex = index;
}

void Renderer::drawAxes(VertexArray& va, Shader& shader, glm::mat4 view, glm::mat4 projection)
{
	va.bind();
	shader.bind();

	glm::mat4 model = glm::mat4(1.0f) * glm::scale(glm::mat4(1.0f), glm::vec3(5));

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

void Renderer::drawMesh(VertexArray& va, Shader& shader, glm::mat4 view, glm::mat4 projection, float scaleFactor)
{
	va.bind();
	shader.bind();

	shader.setUniform4Mat("view", view);
	shader.setUniform4Mat("projection", projection);
	shader.setUniform4Vec("ourColor", glm::vec4(0.5, 0.5, 0.5, 0.0f));

	glm::mat4 model;
	glm::mat4 model2;

	for (float current = -50.0f; current <= 50.0f; current += 1.0f) {

		// center line * scaling
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-50.0f, 0.0f, current))
			* glm::scale(glm::mat4(1.0f), glm::vec3(100));
		shader.setUniform4Mat("model", model);
		GLCall(glDrawArrays(GL_LINES, 0, 2));

		// rotate line * center line * scaling
		model2 = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f))
			* glm::translate(glm::mat4(1.0f), glm::vec3(-50.0f, 0.0f, current))
			* glm::scale(glm::mat4(1.0f), glm::vec3(100));
		shader.setUniform4Mat("model", model2);
		GLCall(glDrawArrays(GL_LINES, 0, 2));
	}

	va.unbind();
	shader.unbind();
}
void Renderer::drawStaticObjects(VertexArray& va, Shader& shader) {
	va.bind();
	shader.bind();
	int index = this->renderIndex;
	vector<vector<int>> staticPositions = { {2,1},{0,2},{0,1} };
	vector<int> staticVector = staticPositions.at(renderIndex);

	glm::mat4 transLeftCorner = glm::translate(glm::mat4(1.0f), glm::vec3(-30.0f, 0.0f, -30.0f));
	glm::mat4 transRightCorner = glm::translate(glm::mat4(1.0f), glm::vec3(30.0f, 0.0f, -30.0f));
	glm::mat4 corner = transLeftCorner;

	//draw objects
	for (int i = 0; i < staticPositions.at(renderIndex).size(); i++) {
		int positionIndex = staticVector.at(i);

		int numCubePieces = modelCubePositions.at(positionIndex).size();
		for (int i = 0; i < numCubePieces; i++) {
			glm::mat4 initialPos = glm::translate(glm::mat4(1.0f), modelPosition.at(positionIndex));
			glm::mat4 modelCubePos = glm::translate(glm::mat4(1.0f), modelCubePositions.at(positionIndex).at(i));

			glm::mat4 model = glm::mat4(1.0f) * corner * initialPos * modelCubePos * modelScale.at(positionIndex);
			shader.setUniform4Mat("model", model);
			glDrawArrays(renderMethod, 0, 36);
		}
		corner = transRightCorner;
	}

	//draw walls
	corner = transLeftCorner;
	for (int i = 0; i < staticPositions.at(renderIndex).size(); i++) {
		int positionIndex = staticVector.at(i);
		int numWallPieces = wallCubePositions.at(positionIndex).size();

		for (int i = 0; i < numWallPieces; i++){
			glm::mat4 initialPos = glm::translate(glm::mat4(1.0f), wallPosition.at(positionIndex));
			glm::mat4 model = glm::mat4(1.0f)
				* initialPos
				* corner
				* glm::translate(glm::mat4(1.0f), wallCubePositions.at(positionIndex).at(i))
				* glm::scale(glm::mat4(1.0f), wallScales.at(positionIndex).at(i));
			
			shader.setUniform4Mat("model", model);
			glDrawArrays(renderMethod, 0, 36);
		}
		corner = transRightCorner;
	}
}
void Renderer::drawObject(VertexArray& va, Shader& shader, vector<glm::mat4> modelRotMat, vector<glm::mat4> modelTransMat, float scaleFactor, glm::vec3 displacement) 
{
	va.bind();
	shader.bind();
	shader.setUniform4Vec("ourColor", glm::vec4(0, 1, 1, 1));
	int numCubePieces = modelCubePositions.at(renderIndex).size();
	float time = (float)glfwGetTime();
		for (int i = 0; i < numCubePieces; i++)
		{
			glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(displacement.x, 0.0f, displacement.z));
			glm::mat4 initialPos = glm::translate(glm::mat4(1.0f), modelPosition.at(renderIndex));

			// unit matrix * scaling input * xz_translation * model_translation (align with hole) * model_rotation * model_cube_scale * model_cube_translation
			glm::mat4 model = glm::mat4(1.0f) * glm::scale(glm::mat4(1.0f), glm::vec3(scaleFactor)) * trans * initialPos * modelRotMat.at(i) * modelScale.at(renderIndex) * modelTransMat.at(i);

			shader.setUniform4Mat("model", model);
			glDrawArrays(renderMethod, 0, 36);
		}
	va.unbind();
	shader.unbind();
}

void Renderer::drawWall(VertexArray& va, Shader& shader, float scaleFactor, glm::vec3 displacement) 
{
	va.bind();
	shader.bind();

	shader.setUniform4Vec("ourColor", glm::vec4(0.63f, 0.63f, 0.63f, 1));
	int numWallPieces = wallCubePositions.at(renderIndex).size();
	for (int i = 0; i < numWallPieces; i++)
	{
		// calculate model matrix for each object and pass it to shader before drawing
		glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(displacement.x, 0.0f, 0.0f));
		glm::mat4 initialPos = glm::translate(glm::mat4(1.0f), wallPosition.at(renderIndex));

		// unit matrix * wall_scale * x_translation * wall_translation (align with XZ plane) * wall_cube_scale * wall_cube_translate
		glm::mat4 model = glm::mat4(1.0f)
			* glm::scale(glm::mat4(1.0f), glm::vec3(scaleFactor))
			* trans
			* initialPos
			* glm::translate(glm::mat4(1.0f), wallCubePositions.at(renderIndex).at(i))
			* glm::scale(glm::mat4(1.0f), wallScales.at(renderIndex).at(i));

		shader.setUniform4Mat("model", model);

		glDrawArrays(renderMethod, 0, 36);
	}
  
	va.unbind();
	shader.unbind();
}

