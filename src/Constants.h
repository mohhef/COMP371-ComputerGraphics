#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

static const double pi = 3.14159265358979323846;

static float deltaTime = 0.0f; // Time increment between frame buffering
static float lastFrame = 0.0f; // Time at which last frame was buffered

static float wallZPos = -10.0f; // wall offset from origin
static int modelIndex = 0; // initial index to display the model

// Window size
static const int HEIGHT = 768;
static const int WIDTH = 1024;

// Cursor position parameters
static float lastX = WIDTH / 2;
static float lastY = HEIGHT / 2;
static bool initialMousePos = true;



//Position(f,f,f) Normal(f,f,f) Texture(f,f)
static float vertices[] =
{
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};

// Verticies for the floor (for texture)
// Position(f,f,f) Normal(f,f,f) Texture(f,f)
static float squareVertices[] =
{
	-0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	0.5f,  0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	-0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	-0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	0.5f,  0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f
};

// Vertices for a line which is used by the axes
static float axesVertices[] =
{
	0.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f
};

// Vertices for a line which is used by the mesh
// Position(f,f,f) Normal(f,f,f) 
static float meshVertices[] = {
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
};

static vector<vector<int>> model1 =
{
	{0, 0, 0, 0, 0},
	{0, 0, 1, 2, 0},
	{0, 1, 0, 2, 0},
	{0, 3, 0, 0, 0},
	{0, 0, 0, 0, 0},
};
static vector<vector<int>> model2 =
{
	{0, 0, 0, 0, 0},
	{0, 0, 1, 2, 0},
	{0, 1, 1, 0, 0},
	{0, 1, 2, 0, 0},
	{0, 0, 0, 1, 0},
	{0, 0, 0, 0, 0},
};
static vector<vector<int>> model3 =
{
	{0, 0, 0, 0, 0},
	{0, 0, 1, 1, 0},
	{0, 2, 0, -2, 0},
	{0, 0, 2, 1, 0},
	{0, 0, 0, 0, 0},
};

static vector<vector<int>> model4 =
{
	{0, 0, 0, 0, 0},
	{0, 1, 1, 1, 0},
	{0, 1, 0, 2, 0},
	{0, 2, 0, 1, 0},
	{0, 0, 0, 0, 0},
};


extern vector<vector<glm::vec3>> modelCubePositions;
extern vector<vector<glm::vec3>> wallCubePositions;

// initial configuration position of wall (to align with XZ plane)
static vector<glm::vec3> wallPosition =
{
	glm::vec3(0.0f, 1.0f, 10.0f),
	glm::vec3(0.0f, 7.5f, 10.0f),
	glm::vec3(0.0f, 4.7f, 10.0f),
	glm::vec3(0.0f, 5.0f, 0.0f)
};


// initial configuration scale of model cubes
static vector<glm::mat4> modelScale
{
	glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f)),
	glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f)),
	glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f)),
	glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f))
};

// initial configuration position of model (to align with hole)
static vector<glm::vec3> modelPosition =
{
	glm::vec3(0.0f, 1.0f, 10.0f),
	glm::vec3(0.0f, 1.0f, 10.0f),
	glm::vec3(0.0f, 1.0f, 10.0f),
	glm::vec3(0.0f, 1.0f, 10.0f)
};

