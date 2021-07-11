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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "Shader.h"
#include "camera.h"

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
using namespace std;

Camera* camera = NULL;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

const int HEIGHT = 768;
const int WIDTH = 1024;

float vertices[] = {
			-0.5f, -0.5f, -0.5f,  
			 0.5f, -0.5f, -0.5f,  
			 0.5f,  0.5f, -0.5f,  
			 0.5f,  0.5f, -0.5f,  
			-0.5f,  0.5f, -0.5f,  
			-0.5f, -0.5f, -0.5f,  

			-0.5f, -0.5f,  0.5f,  
			 0.5f, -0.5f,  0.5f,  
			 0.5f,  0.5f,  0.5f,  
			 0.5f,  0.5f,  0.5f,  
			-0.5f,  0.5f,  0.5f,  
			-0.5f, -0.5f,  0.5f,  

			-0.5f,  0.5f,  0.5f,  
			-0.5f,  0.5f, -0.5f,  
			-0.5f, -0.5f, -0.5f,  
			-0.5f, -0.5f, -0.5f,  
			-0.5f, -0.5f,  0.5f,  
			-0.5f,  0.5f,  0.5f,  

			 0.5f,  0.5f,  0.5f,  
			 0.5f,  0.5f, -0.5f,  
			 0.5f, -0.5f, -0.5f,  
			 0.5f, -0.5f, -0.5f,  
			 0.5f, -0.5f,  0.5f,  
			 0.5f,  0.5f,  0.5f,  

			-0.5f, -0.5f, -0.5f,  
			 0.5f, -0.5f, -0.5f,  
			 0.5f, -0.5f,  0.5f,  
			 0.5f, -0.5f,  0.5f,  
			-0.5f, -0.5f,  0.5f,  
			-0.5f, -0.5f, -0.5f,  

			-0.5f,  0.5f, -0.5f,  
			 0.5f,  0.5f, -0.5f,  
			 0.5f,  0.5f,  0.5f,  
			 0.5f,  0.5f,  0.5f,  
			-0.5f,  0.5f,  0.5f,  
			-0.5f,  0.5f, -0.5f  
};

void processInput(GLFWwindow* window) 
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera->processMovement(KEY::UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera->processMovement(KEY::DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        camera->processMovement(KEY::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camera->processMovement(KEY::RIGHT, deltaTime);
}

int main(int argc, char*argv[])
{
    GLFWwindow* window;
	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create Window and rendering context using GLFW, resolution is WIDTH x HEIGHT
    window = glfwCreateWindow(WIDTH, HEIGHT, "playground", NULL, NULL);
    if (!window)
    {
        cerr << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        cerr << "Failed to create GLEW" << endl;
        glfwTerminate();
        return -1;
    }

	VertexArray va;
	VertexBuffer vb(vertices, 6 * 6 * 5 * sizeof(float));
	VertexBufferLayout layout;

	layout.Push<float>(3);
	va.AddBuffer(vb, layout);

	Shader shader("Basic.shader");
	shader.Bind();
	shader.setUniform4f("ourColor", 1, 0, 0, 1);
    camera = new Camera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	va.Unbind();
	vb.unBind();
    shader.Unbind();
	Renderer renderer;

    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Entering Main Loop
    while(!glfwWindowShouldClose(window))
    {
		renderer.Clear();
        shader.Bind();
        va.Bind();
        vb.Bind();
        // update last frame
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // process input would go here
        processInput(window);

        // update projection matrix and pass to shader
        glm::mat4 projection = glm::perspective(glm::radians(camera->zoom), (float) WIDTH / (float) HEIGHT, 0.1f, 100.0f);
        shader.setUniform4Mat("projection", projection);

        // update view matrix and pass to shader
        glm::mat4 view = camera->getViewMatrix();
        shader.setUniform4Mat("view", view);

        // calculate model matrix and pass to shader
        glm::mat4 model = glm::mat4(1.0f);
        shader.setUniform4Mat("model", model);

        // now render triangles
		glDrawArrays(GL_TRIANGLES, 0, 36);
        // glBindVertexArray(0);

        // End frame
        glfwSwapBuffers(window);
        
        // Detect inputs
        glfwPollEvents();
    }
    
    // Shutdown GLFW
    glfwTerminate();
	return 0;
}
