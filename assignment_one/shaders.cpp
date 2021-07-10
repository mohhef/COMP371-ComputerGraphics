#include <GL/glew.h>
#include <fstream>
#include <sstream>
#include "shaders.h"

using namespace std;

Shader::Shader(string vertexPath, string fragmentPath)
{
	string vertexFileContent;
	string fragmentFileContent;

	ifstream vertexShaderFile;
	ifstream fragmentShaderFile;

	try
	{
		stringstream vertexShaderStream, fragmentShaderStream;
		
		vertexShaderFile.open(vertexPath);
		fragmentShaderFile.open(fragmentPath);

		vertexShaderStream << vertexShaderFile.rdbuf();
		fragmentShaderStream << fragmentShaderFile.rdbuf();

		vertexShaderFile.close();
		fragmentShaderFile.close();

		vertexFileContent = vertexShaderStream.str();
		fragmentFileContent = fragmentShaderStream.str();
	}
	catch (exception e)
	{
		cout << "ERROR: UNABLE TO READ SHADER FILE(S)" << endl;
	}

	const char* vertexShaderCode = vertexFileContent.c_str();
	const char* fragmentShaderCode = fragmentFileContent.c_str();

	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	// vertex shader configuration
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexShaderCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		cout << "ERROR: VERTEX SHADER COMPILATION ISSUE" << endl;
	}

	// fragment shader configuration
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		cout << "ERROR: FRAGMENT SHADER COMPILATION ISSUE" << endl;
	}

	// shader program initialization
	id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	glLinkProgram(id);
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		cout << "ERROR: SHADER PROGRAM LINKING ISSUE" << endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::activate() 
{
	glUseProgram(id);
}

void Shader::setVec4(string name, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w);
}

void Shader::setVec4(string name, glm::vec4 vec) const
{
	glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &vec[0]);
}

void Shader::setMat4(string name, glm::mat4 mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}