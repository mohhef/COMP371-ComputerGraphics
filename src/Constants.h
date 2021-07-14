#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

static const double pi = 3.14159265358979323846;

static float deltaTime = 0.0f;
static float lastFrame = 0.0f;
static float wallZPos = -10.0f;
static int modelIndex = 0;

static const int HEIGHT = 768;
static const int WIDTH = 1024;

static float vertices[] = {
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

static float axesVertices[] = {
			0.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f
};

// initial configuration of cubes for wall
static  vector<vector<glm::vec3>> wallCubePositions =
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
static vector<vector<glm::vec3>> wallScales =
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

// initial configuration position of wall (to align with XZ plane)
static vector<glm::vec3> wallPosition =
{
	glm::vec3(0.0f, 15.0f, 0.0f),
};

// initial configuration of cubes for model
static vector<vector<glm::vec3>> modelCubePositions =
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
static vector<glm::mat4> modelScale
{
	glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f)),
};

// initial configuration position of model (to align with hole)
static vector<glm::vec3> modelPosition =
{
	glm::vec3(-8.0f, 27.5f, 20.0f),
};

