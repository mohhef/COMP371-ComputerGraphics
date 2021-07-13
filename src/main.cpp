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

const double pi = 3.14159265358979323846;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
int modelIndex = 0;

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

vector<vector<glm::vec3>> cubePositions =
{
    {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 1.0f, 0.0f),
        glm::vec3(2.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 0.0f, -2.0f),
    },
};

vector<vector<glm::vec3>> wallPositions =
{
    {
        glm::vec3(0.0f, 0.0f, -30.0f),
        glm::vec3(4.5f, 12.0f, -30.0f),
        glm::vec3(-6.0f, 9.5f, -30.0f),
        glm::vec3(-4.0f, 8.5f, -30.0f),
        glm::vec3(-10.5f, 12.0f, -30.0f),
        glm::vec3(-6.0f, 15.0f, -30.0f)
    },
};

vector<vector<glm::vec3>> wallScales =
{
    {
        glm::vec3(24.0f, 15.0f, 2.0f),
        glm::vec3(15.0f, 9.0f, 2.0f),
        glm::vec3(2.0f, 4.0f, 2.0f),
        glm::vec3(2.0f, 2.0f, 2.0f),
        glm::vec3(3.0f, 9.0f, 2.0f),
        glm::vec3(6.0f, 3.0f, 2.0f)
    },
};

vector<vector<glm::mat4>> modelMatrices;

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

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        for (int i = 0; i < modelMatrices.at(modelIndex).size(); i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 0.25f, 0.0f));
            modelMatrices.at(modelIndex).at(i) = model * modelMatrices.at(modelIndex).at(i);
        }

    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        for (int i = 0; i < modelMatrices.at(modelIndex).size(); i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, -0.25f, 0.0f));
            modelMatrices.at(modelIndex).at(i) = model * modelMatrices.at(modelIndex).at(i);
        }

    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        for (int i = 0; i < modelMatrices.at(modelIndex).size(); i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-0.25f, 0.0f, 0.0f));
            modelMatrices.at(modelIndex).at(i) = modelMatrices.at(modelIndex).at(i) * model;
        }

    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        for (int i = 0; i < modelMatrices.at(modelIndex).size(); i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));
            modelMatrices.at(modelIndex).at(i) = model * modelMatrices.at(modelIndex).at(i);
        }

    }
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

void setObjectModel(Shader* shader)
{
    shader->setUniform4Vec("ourColor", glm::vec4(0, 1, 1, 1));
    int numCubePieces = cubePositions.at(modelIndex).size();    
    float time = (float)glfwGetTime();
    for (int i = 0; i < numCubePieces; i++)
    {
        
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.01f));
        modelMatrices.at(modelIndex).at(i) = model * modelMatrices.at(modelIndex).at(i);
        

        shader->setUniform4Mat("model", modelMatrices.at(modelIndex).at(i));
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

void setWallModel(Shader* shader)
{
    shader->setUniform4Vec("ourColor", glm::vec4(0.63f, 0.63f, 0.63f, 1));
    int numWallPieces = wallPositions.at(modelIndex).size();
    for (int i = 0; i < numWallPieces; i++)
    {
        // calculate model matrix for each object and pass it to shader before drawing
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, wallPositions.at(modelIndex).at(i));
        model = glm::scale(model, wallScales.at(modelIndex).at(i));
        shader->setUniform4Mat("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

int main(int argc, char* argv[])
{
    GLFWwindow* window = initializeWindow();
    {
        VertexArray vA;
        VertexBuffer vB(vertices, sizeof(vertices));
        VertexBufferLayout layout;

        layout.push<float>(3);
        vA.addBuffer(vB, layout);

        Shader* shader = new Shader("vertex_fragment.shader");
        camera = new Camera(glm::vec3(0.0f, 0.0f, 40.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        Renderer renderer;

        glEnable(GL_DEPTH_TEST);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        // initialize model matricies for each cube within each model object
        modelMatrices.resize(cubePositions.size());
        for (int i = 0; i < cubePositions.size(); i++)
        {
            modelMatrices.at(i).resize(cubePositions.at(i).size());

            for (int j = 0; j < cubePositions.at(i).size(); j++)
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, cubePositions.at(i).at(j));
                modelMatrices.at(i).at(j) = model;
            }

        }

        // Entering Main Loop
        while (!glfwWindowShouldClose(window))
        {
            // update last frame
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            // process input would go here
            processInput(window);

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

            // render wall
            setWallModel(shader);

            // render model
            setObjectModel(shader);

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
