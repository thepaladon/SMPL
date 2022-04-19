#pragma once

class IndexBuffer
{
public:
	IndexBuffer(const unsigned int* data, unsigned int count); //size = bytes // count = element count - how Cherno does things
																			  // 24 is the size, 6 is the count
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int getCount() const { return m_Count; }

private:
	unsigned int m_RendererID = 0; //id for every type of cubeMesh we create in OpenGL. 
	unsigned int m_Count = 0; //how many indices it has;
};
