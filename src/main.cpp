//
// COMP 371 Assignment Framework
//
//
// Inspired by the following tutorials:
// - https://learnopengl.com/Getting-started/Hello-Window
// - https://learnopengl.com/Getting-started/Hello-Triangle


#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>

#include "Shader.h"
#include "Camera.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Constants.h"

using namespace std;

// modified throughout run - to reset between runs - possible bound to a single model (modelIndex) - to reset between models
vector<glm::mat4> modelTransMat;
vector<glm::mat4> modelRotMat;
glm::vec3 displacement;
float scaleFactor = 1.0f;

float lastMouseX;
float lastMouseY;

Camera* camera = NULL;

GLFWwindow* initializeWindow()
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create Window and rendering context using GLFW, resolution is WIDTH x HEIGHT
	window = glfwCreateWindow(WIDTH, HEIGHT, "playground", NULL, NULL);
	if (!window)
	{
		cerr << "Failed to create GLFW window" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		cerr << "Failed to create GLEW" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// background color
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	return window;
}

void resetTransMat()
{
	modelTransMat.resize(modelCubePositions.at(modelIndex).size());
	for (int i = 0; i < modelCubePositions.at(modelIndex).size(); i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, modelCubePositions.at(modelIndex).at(i));
		modelTransMat.at(i) = model;
	}
}

void resetRotMat()
{
	modelRotMat.resize(modelCubePositions.at(modelIndex).size());
	for (int i = 0; i < modelCubePositions.at(modelIndex).size(); i++)
		modelRotMat.at(i) = glm::mat4(1.0f);
}

void resetModel()
{
	resetTransMat();
	resetRotMat();
	glfwSetTime(0.0f);
	displacement = glm::vec3();
	scaleFactor = 1.0f;
}

void processInput(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// closes window
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// rotate of camera around world
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		camera->processMovement(KEY::UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		camera->processMovement(KEY::DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		camera->processMovement(KEY::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		camera->processMovement(KEY::RIGHT, deltaTime);

	// reset camera
	if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS)
		camera = new Camera(glm::vec3(modelPosition.at(modelIndex).x, modelPosition.at(modelIndex).y, 100.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f));

	// scale model
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS && scaleFactor < 1.25f)
		scaleFactor += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && scaleFactor > 0.75f)
		scaleFactor -= 0.01f;

	// rotate model
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		if (mode == GLFW_MOD_SHIFT && displacement.z != -100.f)
			displacement.z -= 0.50f;
		else
		{
			for (int i = 0; i < modelRotMat.size(); i++)
			{
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::rotate(model, glm::radians(5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				modelRotMat.at(i) = model * modelRotMat.at(i);
			}
		}
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		if (mode == GLFW_MOD_SHIFT && displacement.z != 20.f)
			displacement.z += 0.50f;
		else
		{
			for (int i = 0; i < modelRotMat.size(); i++)
			{
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				modelRotMat.at(i) = model * modelRotMat.at(i);
			}
		}
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		if (mode == GLFW_MOD_SHIFT && displacement.x != -20.f)
			displacement.x -= 0.50f;
		else
		{
			for (int i = 0; i < modelRotMat.size(); i++)
			{
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				modelRotMat.at(i) = model * modelRotMat.at(i);
			}
		}
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		if (mode == GLFW_MOD_SHIFT && displacement.x != 20.f)
			displacement.x += 0.50f;
		else
		{
			for (int i = 0; i < modelRotMat.size(); i++)
			{
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				modelRotMat.at(i) = model * modelRotMat.at(i);
			}
		}
	}

	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		Renderer::getInstance().setRenderMethod(GL_POINTS);

	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		Renderer::getInstance().setRenderMethod(GL_LINES);

	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		Renderer::getInstance().setRenderMethod(GL_TRIANGLES);
}

void processMouse(GLFWwindow* window, double xpos, double  ypos)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE
		&& glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE
		&& glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_RELEASE)
	{
		initialMousePos = true;
		return;
	}

	if (initialMousePos)
	{
		lastMouseX = xpos;
		lastMouseY = ypos;
		initialMousePos = false;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		float yOffset = lastMouseY - ypos;

		lastMouseX = xpos;
		lastMouseY = ypos;

		camera->zoomCamera(yOffset);
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		float xOffset = lastMouseX - xpos;

		lastMouseX = xpos;
		lastMouseY = ypos;

		camera->panCamera(xOffset);
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
	{
		float yOffset = lastMouseY - ypos;

		lastMouseX = xpos;
		lastMouseY = ypos;

		camera->tiltCamera(yOffset);
	}
}

int main(int argc, char* argv[])
{
	GLFWwindow* window = initializeWindow();
	{
		// Setup for models
		VertexArray vA;
		VertexBuffer vB(vertices, sizeof(vertices));
		VertexBufferLayout layout;

		layout.push<float>(3);
		vA.addBuffer(vB, layout);

		// Setup for axes
		VertexArray vaAxes;
		VertexBuffer vbAxes(axesVertices, 3 * 2 * sizeof(float));
		VertexBufferLayout layoutAxes;
		layoutAxes.push<float>(3);
		vaAxes.addBuffer(vbAxes, layoutAxes);

		// Setup for mesh
		VertexArray vaMesh;
		VertexBuffer vbMesh(meshVertices, 3 * 2 * sizeof(float));
		VertexBufferLayout layoutMesh;
		layoutMesh.push<float>(3);
		vaMesh.addBuffer(vbMesh, layoutMesh);

		Shader* shader = new Shader("vertex_fragment.shader");
		Shader* axesShader = new Shader("axes.shader");
		Shader* meshShader = new Shader("vertex_fragment.shader");

		camera = new Camera(glm::vec3(modelPosition.at(modelIndex).x, modelPosition.at(modelIndex).y, 100.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f));

		Renderer& renderer = Renderer::getInstance();

		glEnable(GL_DEPTH_TEST);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// initialize model matricies for each cube within each model 
		resetModel();
		glfwSetKeyCallback(window, processInput);
		glfwSetCursorPosCallback(window, processMouse);

		// Entering Main Loop
		while (!glfwWindowShouldClose(window))
		{
			// update last frame
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			renderer.clear();

			shader->bind();
			vA.bind();
			vB.bind();

			shader->setUniform4f("ourColor", 1, 0, 0, 1);

			// update projection matrix and pass to shader
			glm::mat4 projection = glm::perspective(glm::radians(camera->zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 200.0f);
			shader->setUniform4Mat("projection", projection);

			// update view matrix and pass to shader
			glm::mat4 view = camera->getViewMatrix();
			shader->setUniform4Mat("view", view);


			// render 
			renderer.drawWall(vA, *shader, scaleFactor, displacement);
			renderer.drawObject(vA, *shader, modelRotMat, modelTransMat, scaleFactor, displacement);
			renderer.drawAxes(vaAxes, *axesShader, view, projection);
			renderer.drawMesh(vaMesh, *meshShader, view, projection, scaleFactor);

			// End frame
			glfwSwapBuffers(window);

			// Detect inputs
			glfwPollEvents();
		}
	}

	// Shutdown GLFW
	glfwTerminate();
	return 0;
}
