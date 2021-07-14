#include "Renderer.h"
#include <iostream>
#include "Shader.h"

void glClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool glLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL Error] (" << error << ")" << function << " " <<
			" " << file << " " << line << std::endl;
		return false;
	}
	return true;
}

void Renderer::clear() const
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::draw(const VertexArray& va, const Shader& shader) const
{
	shader.bind();
	va.bind();
	GLCall(glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr));
}

void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	shader.bind();
	va.bind();
	ib.bind();
	GLCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));

}

void Renderer::DrawAxes(VertexArray& va, Shader& shader)
{
	va.bind();
	shader.bind();
	GLCall(glDrawArrays(GL_LINES, 0, 2));
}