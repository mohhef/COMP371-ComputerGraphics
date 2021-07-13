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
float wallZPos = -10.0f;
int modelIndex = 0;

float scaleFactor = 1.0f;

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

// initial configuration of cubes for wall
vector<vector<glm::vec3>> wallCubePositions =
{
    {
        glm::vec3(0.0f, 0.0f, wallZPos),
        glm::vec3(4.5f, 12.0f, wallZPos),
        glm::vec3(-6.0f, 9.5f, wallZPos),
        glm::vec3(-4.0f, 8.5f, wallZPos),
        glm::vec3(-10.5f, 12.0f, wallZPos),
        glm::vec3(-6.0f, 15.0f, wallZPos)
    },
};

// initial configuration scale of wall cubes
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

// initial configuration of cubes for model
vector<vector<glm::vec3>> modelCubePositions =
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

// initial configuration scale of model cubes
vector<glm::mat4> modelScale
{
    glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f)),
};

// initial configuration position of model (to align with hole)
vector<glm::vec3> modelPosition =
{
    glm::vec3(-8.0f, 12.5f, 20.0f),
};

// modified throughout run - to reset between runs - bound to a single model (modelIndex)
vector<glm::mat4> modelTransMat;

// modified throughout run - to reset between runs - bound to a single model (modelIndex)
vector<glm::mat4> modelRotMat;

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
    int numCubePieces = modelCubePositions.at(modelIndex).size();    
    float time = (float)glfwGetTime();
    for (int i = 0; i < numCubePieces; i++)
    {
        glm::mat4 transZ = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -(float)glfwGetTime()));
        glm::mat4 initialPos = glm::translate(glm::mat4(1.0f), modelPosition.at(modelIndex));
        
        // unit matrix * scaling input * z_translation * model_translation (align with hole) * model_rotation * model_cube_scale * model_cube_translation
        glm::mat4 model = glm::mat4(1.0f) * glm::scale(glm::mat4(1.0f), glm::vec3(scaleFactor)) * transZ * initialPos * modelRotMat.at(i) * modelScale.at(modelIndex) * modelTransMat.at(i);

        shader->setUniform4Mat("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

void setWallModel(Shader* shader)
{
    shader->setUniform4Vec("ourColor", glm::vec4(0.63f, 0.63f, 0.63f, 1));
    int numWallPieces = wallCubePositions.at(modelIndex).size();
    for (int i = 0; i < numWallPieces; i++)
    {
        // calculate model matrix for each object and pass it to shader before drawing
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(scaleFactor));
        model = glm::translate(model, wallCubePositions.at(modelIndex).at(i));
        model = glm::scale(model, wallScales.at(modelIndex).at(i));

        // unit matrix * wall_scale * wall_cube_scale * wall_cube_translate
        shader->setUniform4Mat("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
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
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelRotMat.at(i) = model;
    }
}

void resetModel()
{
    resetTransMat();
    resetRotMat();
    glfwSetTime(0.0f);
    scaleFactor = 1.0f;
}

void processInput(GLFWwindow* window)
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

    // reset model
    if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS)
        resetModel();

    // scale model
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
    {
        if (scaleFactor < 1.25f)
            scaleFactor += 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        if (scaleFactor > 0.75f)
            scaleFactor -= 0.01f;
    }

    // rotate model
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        for (int i = 0; i < modelRotMat.size(); i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::rotate(model, glm::radians(5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            modelRotMat.at(i) = model * modelRotMat.at(i);
        }
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        for (int i = 0; i < modelRotMat.size(); i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            modelRotMat.at(i) = model * modelRotMat.at(i);
        }
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        for (int i = 0; i < modelRotMat.size(); i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            modelRotMat.at(i) = model * modelRotMat.at(i);
        }
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        for (int i = 0; i < modelRotMat.size(); i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            modelRotMat.at(i) = model * modelRotMat.at(i);
        }
    }
}

int main(int argc, char* argv[])
{
    GLFWwindow* window = initializeWindow();
    {
        VertexArray vA;
        VertexBuffer vB(vertices, sizeof(vertices));
        VertexBufferLayout layout;

        cout << sizeof(vertices) << endl;
        cout << 6 * 6 * 3 * sizeof(float) << endl;

        layout.push<float>(3);
        vA.addBuffer(vB, layout);

        Shader* shader = new Shader("vertex_fragment.shader");
        camera = new Camera(glm::vec3(modelPosition.at(modelIndex).x, modelPosition.at(modelIndex).y, 50.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        Renderer renderer;

        glEnable(GL_DEPTH_TEST);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        // initialize model matricies for each cube within each model object
        resetTransMat();
        resetRotMat();

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
