#pragma once
class IndexBuffer
{
private:
	//numeric id that keeps track of opengl objects
	unsigned int m_RendererID;
	unsigned int m_Count;
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void unBind() const;

	inline unsigned int GetCount() const { return m_Count; }
};

