#include "FrameBuffer.h"
#include "Renderer.h"

// Vertex buffer constructor
FrameBuffer::FrameBuffer(const void* data, unsigned int size)
{
	glGenBuffers(1, &id);
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

// Vertex buffer destructor
FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &id);
}

// Vertex buffer bind
void FrameBuffer::bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, id);
}

// Vertex buffer unbind
void FrameBuffer::unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
