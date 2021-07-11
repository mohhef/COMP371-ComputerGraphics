#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <unordered_map>

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader 
{
private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	Shader(const std::string& v);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void setUniform4f(const std::string& name,  float x, float y, float z, float w) ;
	void setUniform4Vec(const std::string& name, glm::vec4 vec) ;
	void setUniform4Mat(const std::string& name, glm::mat4 mat) ;

	unsigned int  CompileShader(unsigned int type, const std::string& source);
	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int GetUniformLocation(const std::string& name) ;
};