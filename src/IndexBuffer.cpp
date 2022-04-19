#include "IndexBuffer.h"

#include "Counter.h"
//#include "Renderer.h"


#ifdef RASPBERRY
#include <GLES3/gl3.h>
#elif WIN32
#include <GL/glew.h>
#endif

IndexBuffer::IndexBuffer(const unsigned* data, unsigned count)
	:m_Count(count)
{
	glGenBuffers(1, &m_RendererID); //generates a buffer and gives us an id
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID); //this is what it's going to draw, this is the STATE
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * 4 , data, GL_STATIC_DRAW); //THIS SIZE IS IN BITES (use https://docs.gl/ for the documentation of OpenGl)
	Counter::GetCounter().addIndexID(m_RendererID);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID); //this is what it's going to draw, when you call "Draw".
}

void IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //Unbinding it (setting it to 0) means this WON'T Draw when you call draw 

}
