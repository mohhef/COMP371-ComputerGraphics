#pragma once
class VertexBuffer
{
private:
	//numeric id that keeps track of opengl objects
	unsigned int m_RendererID;
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind()const;
	void unBind() const;
};

