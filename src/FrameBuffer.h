#pragma once
class FrameBuffer
{
public:
	FrameBuffer(const void* data, unsigned int size);
	~FrameBuffer();

	void bind()const;
	void unbind() const;

private:
	// vertex buffer id
	unsigned int id;
};

