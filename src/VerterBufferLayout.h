#pragma once
#include <cstdio>
#include <vector>

#ifdef RASPBERRY
#include <GLES3/gl3.h>
#elif WIN32
#include <GL/glew.h>
#endif


struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int getSizeOfType(unsigned int type)
	{
		switch(type)
		{
			case GL_FLOAT:			return 4;
			case GL_UNSIGNED_INT:	return 4;
			case GL_UNSIGNED_BYTE:	return 1;
		}

		printf("UNABLE TO GET SIZE OF TYPE in VertexBufferLayout.h \n");
		return 0;
	}

};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;
public:
	VertexBufferLayout()
	: m_Stride(0) {};

	void PushFloat(unsigned count)
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
	}

	std::vector<VertexBufferElement> GetElements() const& { return m_Elements; }
	[[nodiscard]] int getStride() const { return m_Stride; }

};