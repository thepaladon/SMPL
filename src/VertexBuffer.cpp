#include "VertexBuffer.h"

#include "Counter.h"

#ifdef RASPBERRY
#include <GLES3/gl3.h>
#elif WIN32
#include <GL/glew.h>
#endif

VertexBuffer::VertexBuffer(const void* data, unsigned size)
{
	glGenBuffers(1, &m_RendererID); //generates a buffer and gives us an id
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); //this is what it's going to draw, this is the STATE
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW); //THIS SIZE IS IN BITES (use https://docs.gl/ for the documentation of OpenGl)
	Counter::GetCounter().addVertexBufferID(m_RendererID);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); //this is what it's going to draw, when you call "Draw".

}

void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0); //Unbinding it (setting it to 0) means this WON'T Draw when you call draw 

}
