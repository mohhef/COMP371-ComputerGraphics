//
// COMP 371 Assignment Framework
//
//
// Inspired by the following tutorials:
// - https://learnopengl.com/Getting-started/Hello-Window
// - https://learnopengl.com/Getting-started/Hello-Triangle


#define GLEW_STATIC 1
#define STB_IMAGE_IMPLEMENTATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>

#include "Shader.h"
#include "Camera.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Constants.h"
#include "stb_image.h" // Reference: Lab 4 (COMP 353) and LearnOpenGL; Source: https://github.com/nothings/stb/blob/master/stb_image.h

using namespace std;

// Modified throughout run and to reset between runs.
// Possibly bound to a single model (modelIndex)
vector<glm::mat4> modelTransMat;
vector<glm::mat4> modelRotMat;
glm::vec3 displacement;
float scaleFactor = 1.0f;
bool combinedRot = false;

// Cursor positions for mouse inputs
float lastMouseX;
float lastMouseY;

Camera* camera = NULL;

// function calls
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
GLFWwindow* initializeWindow();
void resetTransMat();
void resetRotMat();
void resetModel();
void processInput(GLFWwindow* window, int key, int scancode, int action, int mode);
void processMouse(GLFWwindow* window, double xpos, double  ypos);
unsigned int loadTexture(char const* path);

// main function
int main(int argc, char* argv[])
{
	GLFWwindow* window = initializeWindow();
	{
		// Setup for models
		VertexArray vA;
		VertexBuffer vB(vertices, sizeof(vertices));
		VertexBufferLayout layout;
		layout.push<float>(3);
		layout.push<float>(3);
		layout.push<float>(2);
		vA.addBuffer(vB, layout);

		// Setup for lighting
		VertexArray vaLightingSource;
		VertexBuffer vblightingSource(vertices, sizeof(vertices));
		VertexBufferLayout layoutLightingSource;
		layoutLightingSource.push<float>(3);
		layoutLightingSource.push<float>(3);
		layoutLightingSource.push<float>(2);
		vaLightingSource.addBuffer(vblightingSource, layoutLightingSource);

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

		// Create shader instances
		Shader* shader = new Shader("vertex_fragment.shader");
		Shader* axesShader = new Shader("axes.shader");
		Shader* meshShader = new Shader("axes.shader");
		Shader* lightingSourceShader = new Shader("lightingSource.shader");

		// Renddering setup
		Renderer& renderer = Renderer::getInstance();
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		// Create camera instance
		// Position: behind model, along Z-axis.
		// Target: world origin (initially)
		camera = new Camera(glm::vec3(modelPosition.at(modelIndex).x, modelPosition.at(modelIndex).y, 100.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f));

		// Position of the light source
		glm::vec3 lightPos(0.0, 40.0f, 20.0f);

		// Initialize model matricies for each cube within each model 
		resetModel();
		glfwSetKeyCallback(window, processInput);
		glfwSetCursorPosCallback(window, processMouse);

		// load texture ids
		unsigned int brickTextureID = loadTexture(".\\brick.jpg");
		unsigned int tileTextureID = loadTexture(".\\tiles.jpg");

		// Entering main loop
		while (!glfwWindowShouldClose(window))
		{
			// Update last frame
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			// Clear color and depth buffers
			renderer.clear();

			glm::mat4 projection = glm::perspective(glm::radians(camera->zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 200.0f);
			glm::mat4 view = camera->getViewMatrix();

			// Render each object (wall, model, static models, axes, and mesh floor)
			renderer.drawObject(vA, *shader, modelRotMat, modelTransMat, scaleFactor, displacement, view, projection, lightPos, camera->position);
			renderer.drawWall(vA, *shader, modelRotMat, scaleFactor, displacement, view, projection, lightPos, camera->position, brickTextureID);
			renderer.drawStaticObjects(vA, *shader, view, projection, lightPos, camera->position);
			renderer.drawLightingSource(vaLightingSource, *lightingSourceShader, lightPos, view, projection);
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

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

GLFWwindow* initializeWindow()
{
	GLFWwindow* window;

	// Initialize the library
	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GL_DEPTH_BITS, 24);

	// Create Window and rendering context using GLFW, resolution is WIDTH x HEIGHT
	window = glfwCreateWindow(WIDTH, HEIGHT, "playground", NULL, NULL);
	if (!window)
	{
		cerr << "Failed to create GLFW window" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSwapInterval(1);

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		cerr << "Failed to create GLEW" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Background color
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	return window;
}

// Reset translation matrix for each model's cube.
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

// Reset model's rotation matrix.
void resetRotMat()
{
	Renderer::getInstance().setRenderCombinedRot(true);
	modelRotMat.resize(modelCubePositions.at(modelIndex).size());
	for (int i = 0; i < modelCubePositions.at(modelIndex).size(); i++)
		modelRotMat.at(i) = glm::mat4(1.0f);
}

// Reset model to initial position
void resetModel()
{
	resetTransMat();
	resetRotMat();
	glfwSetTime(0.0f);
	displacement = glm::vec3();
	scaleFactor = 1.0f;
}

// Handle all keyboard inputs
void processInput(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//Toggle rotate object only or wall & object
	mode == GLFW_MOD_CONTROL?
		Renderer::getInstance().setRenderCombinedRot(true):Renderer::getInstance().setRenderCombinedRot(false);

	// Closes window
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// Toggle between models (1-2-3)
	if (key == GLFW_KEY_1 || key == GLFW_KEY_2 || key == GLFW_KEY_3) {
		if (key == GLFW_KEY_1) {
			modelIndex = 0;
			Renderer::getInstance().setRenderIndex(modelIndex);
		}
		if (key == GLFW_KEY_2) {
			modelIndex = 1;
			Renderer::getInstance().setRenderIndex(modelIndex);
		}
		if (key == GLFW_KEY_3) {
			modelIndex = 2;
			Renderer::getInstance().setRenderIndex(modelIndex);
		}
		resetModel();
		camera = new Camera(glm::vec3(modelPosition.at(modelIndex).x, modelPosition.at(modelIndex).y, 100.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f));
	}
	
	// Camera rotation around world axis (UP/DOWN/LEFT/RIGHT)
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		camera->processMovement(KEY::UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		camera->processMovement(KEY::DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		camera->processMovement(KEY::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		camera->processMovement(KEY::RIGHT, deltaTime);

	// Camera reset (HOME)
	if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS)
		camera = new Camera(glm::vec3(modelPosition.at(modelIndex).x, modelPosition.at(modelIndex).y, 100.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f));

	// Reset model (SPACEBAR)
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		resetModel();

	// Scale model and wall (U/J)
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS && scaleFactor < 1.25f)
		scaleFactor += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && scaleFactor > 0.75f)
		scaleFactor -= 0.01f;

	// Model displacement (W/S/A/D) and rotation (w/s/a/d), the latter of which around it's own axis.
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
	if (key == GLFW_KEY_Q)
	{
		for (int i = 0; i < modelRotMat.size(); i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			modelRotMat.at(i) = model * modelRotMat.at(i);
		}
	}
	if (key == GLFW_KEY_E)
	{
		for (int i = 0; i < modelRotMat.size(); i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			modelRotMat.at(i) = model * modelRotMat.at(i);
		}
	}
	// Toggle rendering mode between point, line and fill mode (P/L/T)
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

// Function for processing mouse input
void processMouse(GLFWwindow* window, double xpos, double  ypos)
{
	// Required for continous call (holding down button)
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE
		&& glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE
		&& glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_RELEASE)
	{
		initialMousePos = true;
		return;
	}

	// Required to prevent jump when toggling mouse button
	// Note that call can be made from different cursor position
	if (initialMousePos)
	{
		lastMouseX = xpos;
		lastMouseY = ypos;
		initialMousePos = false;
	}

	// Camera zoom (LMB)
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		float yOffset = lastMouseY - ypos;

		lastMouseX = xpos;
		lastMouseY = ypos;

		camera->zoomCamera(yOffset);
	}

	// Camera pan (RMB)
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		float xOffset = lastMouseX - xpos;

		lastMouseX = xpos;
		lastMouseY = ypos;

		camera->panCamera(xOffset);
	}

	// Camera tilt (MMB)
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
	{
		float yOffset = lastMouseY - ypos;

		lastMouseX = xpos;
		lastMouseY = ypos;

		camera->tiltCamera(yOffset);
	}
}

// Reference: Lab 4 (COMP 353) and LearnOpenGL
unsigned int loadTexture(char const* path)
{
	// step 1: create and bind textures
	unsigned int id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	// step 2: filter paramaters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	// step 3: load textures
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (!data)
	{
		cout << "Texture failed to load at path: " << path << endl;
		stbi_image_free(data);
	}

	// step 4: upload texture to GPU
	GLenum format;
	if (nrChannels == 1)
		format = GL_RED;
	else if (nrChannels == 3)
		format = GL_RGB;
	else if (nrChannels == 4)
		format = GL_RGBA;

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	// step 5: free all resources
	stbi_image_free(data);

	return id;
}