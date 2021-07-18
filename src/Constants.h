#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

static const double pi = 3.14159265358979323846;

static float deltaTime = 0.0f;
static float lastFrame = 0.0f;
static float wallZPos = -10.0f; // wall offset from origin
static int modelIndex = 0; // initial index to display the model

// window size
static const int HEIGHT = 768;
static const int WIDTH = 1024;

static float lastX = WIDTH / 2;
static float lastY = HEIGHT / 2;
static float pitch = 0.0f;
static float yaw = -90.0f;
static bool initialMousePos = true;

// Vertices used to draw a cube
static float vertices[] =
{
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

// Vertices for a line which is used by the axes
static float axesVertices[] =
{
	0.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f
};

// Vertices for a line which is used by the mesh
static float meshVertices[] = {
	0.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f
};

// initial configuration of cubes for wall
static  vector<vector<glm::vec3>> wallCubePositions =
{
	{
		glm::vec3(0.0f, 0.0f, wallZPos),
		glm::vec3(4.0f, 4.0f, wallZPos),
		glm::vec3(-4.0f, 4.0f, wallZPos),
		glm::vec3(0.0f, 8.0f, wallZPos),
		glm::vec3(2.0f, 2.0f, wallZPos),
		glm::vec3(2.0f, 6.0f, wallZPos),
		glm::vec3(-2.0f, 6.0f, wallZPos),
	},
	{
		glm::vec3(0.0f, 0.0f, wallZPos),
		glm::vec3(4.5f, 12.0f, wallZPos),
		glm::vec3(-6.0f, 9.5f, wallZPos),
		glm::vec3(-4.0f, 8.5f, wallZPos),
		glm::vec3(-10.5f, 12.0f, wallZPos),
		glm::vec3(-6.0f, 15.0f, wallZPos)
	},
	{
		glm::vec3(0.0f, 0.3f, wallZPos),
		glm::vec3(2.0f, 7.3f, wallZPos),
		glm::vec3(6.4f, 6.3f, wallZPos),
		glm::vec3(7.6f, 12.3f, wallZPos),
		glm::vec3(-5.5f, 11.3f, wallZPos),
		glm::vec3(2.0f, 15.45f, wallZPos),
		glm::vec3(4.0f, 10.5f, wallZPos),
	},
};

// initial configuration scale of wall cubes
static vector<vector<glm::vec3>> wallScales =
{
	{
		glm::vec3(6.0f, 2.0f, 2.0f),
		glm::vec3(2.0f, 10.0f, 2.0f),
		glm::vec3(2.0f, 10.0f, 2.0f),
		glm::vec3(6.0f, 2.0f, 2.0f),
		glm::vec3(2.0f, 2.0f, 2.0f),
		glm::vec3(2.0f, 2.0f, 2.0f),
		glm::vec3(2.0f, 2.0f, 2.0f),
	},
	{
		glm::vec3(24.0f, 15.0f, 2.0f),
		glm::vec3(15.0f, 9.0f, 2.0f),
		glm::vec3(2.0f, 4.0f, 2.0f),
		glm::vec3(2.0f, 2.0f, 2.0f),
		glm::vec3(3.0f, 9.0f, 2.0f),
		glm::vec3(6.0f, 3.0f, 2.0f)
	},
	{
		glm::vec3(20.0f, 10.0f, 1.0f),
		glm::vec3(1.8f, 4.0f, 1.0f),
		glm::vec3(7.2f, 2.0f, 1.0f),
		glm::vec3(4.8f, 10.0f, 1.0f),
		glm::vec3(9.0f, 12.0f, 1.0f),
		glm::vec3(6.0f, 3.7f, 1.0f),
		glm::vec3(2.0f, 2.0f, 1.0f),
	},
};

// initial configuration position of wall (to align with XZ plane)
static vector<glm::vec3> wallPosition =
{
	glm::vec3(0.0f, 1.0f, 10.0f),
	glm::vec3(0.0f, 7.5f, 10.0f),
	glm::vec3(0.0f, 4.7f, 10.0f),
};

// initial configuration of cubes for model
static vector<vector<glm::vec3>> modelCubePositions =
{
	{
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 2.0f),
		glm::vec3(-1.0f, -1.0f,0.0f),
		glm::vec3(-1.0f, -1.0f,-1.0f),
	},
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
	{
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 0.0f),
		glm::vec3(2.0f, 2.0f, 0.0f),
		glm::vec3(3.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 2.0f, 2.0f),
		glm::vec3(0.0f, 1.0f, 2.0f),
		glm::vec3(0.0f, 2.0f, 1.0f),
	},
};

// initial configuration scale of model cubes
static vector<glm::mat4> modelScale
{
	glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f)),
	glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f)),
	glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f)),
};

// initial configuration position of model (to align with hole)
static vector<glm::vec3> modelPosition =
{
	glm::vec3(0.0f, 5.0f, 10.0f),
	glm::vec3(-8.0f, 20.0f, 10.0f),
	glm::vec3(0.0f, 17.2f, 10.0f)
};