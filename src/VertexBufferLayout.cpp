#include "VertexBufferLayout.h"


unsigned int VertexBufferElement::getSizeOfType(unsigned int type) 
{
	switch (type)
	{
	case GL_FLOAT: return 4;
	case GL_UNSIGNED_INT: return 4;
	case GL_UNSIGNED_BYTE: return 1;
	}
	ASSERT(false);
	return 0;
}

VertexBufferLayout::VertexBufferLayout()
{
	stride = 0;
}

const std::vector<VertexBufferElement> VertexBufferLayout::getElements() const
{
	return elements;
}

unsigned int VertexBufferLayout::getStride() const
{
	return stride;
}

template<typename T>
void VertexBufferLayout::push(unsigned int count) 
{
	static_assert(false);
}

template<>
void VertexBufferLayout::push<float>(unsigned count) 
{
	elements.push_back({ GL_FLOAT,count, GL_FALSE });
	stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
}

template<>
void VertexBufferLayout::push<unsigned int>(unsigned int count) 
{
	elements.push_back({ GL_UNSIGNED_INT,count, GL_FALSE });
	stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
}

template<>
void VertexBufferLayout::push<unsigned char>(unsigned int count) 
{
	elements.push_back({ GL_UNSIGNED_BYTE,count, GL_TRUE });
	stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
}