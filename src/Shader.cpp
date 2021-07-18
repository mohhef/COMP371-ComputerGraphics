#include <stdio.h>
#include <sstream>
#include <iostream>
#include <string>
#include <fstream>
#include "Shader.h"
#include "Renderer.h"


// Shader constructor using the filepath
Shader::Shader(const std::string& filePath)
{
	this->filePath = filePath;
	// Creating a program source object to store the shaders at the specified file path
	ShaderProgramSource source = parseShader(filePath);
	std::cout << source.vertexSource << std::endl;
	std::cout << source.fragmentSource << std::endl;
	id = createShader(source.vertexSource, source.fragmentSource);

}

// Destructor
Shader::~Shader()
{
	GLCall(glDeleteProgram(id));
}

// Method for creating the shader and returning its id
unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	// create shader program
	unsigned int program = glCreateProgram();

	// assign object id to each shader
	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	// attach shaders
	glAttachShader(program, vs);
	glAttachShader(program, fs);

	// link shaders
	glLinkProgram(program);
	glValidateProgram(program);

	// since they have been linked already we can delete
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

unsigned int Shader::compileShader(unsigned int type, const std::string& source) {

	// assign object id
	unsigned int id = glCreateShader(type);
	
	// returns a pointer to the data
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	// verify for errors
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		
		// lets you allocate on the stack dynamically
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Error: Unable to compile" << (type == GL_VERTEX_SHADER ? "VERTEX" : "Fragment") << std::endl;
	}

	return id;
}

// Method for parsing the shader at the specified file path
ShaderProgramSource Shader::parseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	// Read through the program line by line
	while (getline(stream, line)) {

		// If the line contains #shader vertex or #shader fragment, add the following lines to the appropriate array for storage
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			ss[(int)type] << line << "\n";
		}
	}
	return { ss[0].str(), ss[1].str() };
}

// Bind the shader
void Shader::bind() const
{
	GLCall(glUseProgram(id));
}

// unbind the shader
void Shader::unbind() const
{
	GLCall(glUseProgram(0));
}

void Shader::setUniform1i(const std::string& name, int i) {
	glUniform1i(getUniformLocation(name), i);
}

void Shader::setUniform4f(const std::string& name, float x, float y, float z, float w)
{
	glUniform4f(getUniformLocation(name), x, y, z, w);
}

void Shader::setUniform4Vec(const std::string& name, glm::vec4 vec) 
{
	glUniform4fv(getUniformLocation(name), 1, &vec[0]);
}

void Shader::setUniform4Mat(const std::string& name, glm::mat4 mat) 
{
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

unsigned int Shader::getUniformLocation(const std::string& name)
{
	if (uniformLocationCache.find(name) != uniformLocationCache.end())
		return uniformLocationCache[name];
	GLCall(int location = glGetUniformLocation(id, name.c_str()));
	if (location == -1)
		std::cout << "Error: unable to locate uniform " << name << std::endl;
	uniformLocationCache[name] = location;
	return location;
}
