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

#include "Axes.h"

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


    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        cerr << "Failed to create GLEW" << endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    return window;
}

int main(int argc, char* argv[])
{
    GLFWwindow* window = initializeWindow();
    //this scope is to prevent infinite lope when terminating opengl (https://www.youtube.com/watch?v=bTHqmzjm2UI&list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2&index=13)
    {
		Axes axes;

		float axesVertices[] = {
	        0.0f, 0.0f, 0.0f,
	        1.0f, 0.0f, 0.0f
		};

		VertexArray va;
		VertexBuffer vb(axesVertices, 3 * 2 * sizeof(float));
	    VertexBufferLayout layout;
		layout.Push<float>(3);
		va.AddBuffer(vb, layout);
        
        
        //VertexArray va;
        //VertexBuffer vb(vertices, 6 * 6 * 3 * sizeof(float));
		//VertexBufferLayout layout;

        //layout.Push<float>(3);
        //va.AddBuffer(vb, layout);

        //Shader shader("Basic.shader");
        Shader axesShader("Axes.shader");
        camera = new Camera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        Renderer renderer;

        glEnable(GL_DEPTH_TEST);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		

        // Entering Main Loop
        while (!glfwWindowShouldClose(window))
        {
            // update last frame
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            // process input would go here
            processInput(window);

            renderer.Clear();
			axesShader.Bind();

            //shader.setUniform4f("ourColor", 1, 0, 0, 1);

            // update projection matrix and pass to shader
            glm::mat4 projection = glm::perspective(glm::radians(camera->zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
            axesShader.setUniform4Mat("projection", projection);
            // update view matrix and pass to shader
            glm::mat4 view = camera->getViewMatrix();
            axesShader.setUniform4Mat("view", view);
            // calculate model matrix and pass to shader
			glm::mat4 model = glm::mat4(1.0f);
            for (int i = 0; i < 3; i++) {
                switch (i) {
                case 0:
                    axesShader.setUniform1i("xAxis", 1);
                    axesShader.setUniform4Vec("ourColor", glm::vec4(1.0, 0.0, 0.0, 1.0));
                    break;

                case 1:
                    model = glm::rotate(model, glm::radians(-85.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                    axesShader.setUniform1i("yAxis", 1);
                    axesShader.setUniform4Vec("ourColor", glm::vec4(0.0, 1.0, 0.0, 1.0));
                    break;

                case 2:
                    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                    axesShader.setUniform1i("zAxis", 1);
					axesShader.setUniform4Vec("ourColor", glm::vec4(0.0, 0.0, 1.0, 1.0));
                    break;
                }
				axesShader.setUniform4Mat("model", model);
				renderer.DrawAxes(va, axesShader);
            }
			
            // now render triangles

            //glDrawArrays(GL_TRIANGLES, 0, 36);

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
