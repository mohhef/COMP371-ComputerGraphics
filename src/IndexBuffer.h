#pragma once

// class works in conjuction with Element Object Buffers
class IndexBuffer
{
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void bind() const;
	void unbind() const;

	unsigned int getCount() const;

private:
	// opengl object id
	unsigned int id;
	// total number of indexes
	unsigned int count;
};

