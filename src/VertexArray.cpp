#include "VertexArray.h"

#include "Counter.h"
#include "VerterBufferLayout.h"
#pragma warning( disable : 4312 )

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_RendererID);
    Counter::GetCounter().addVertexArrayID(m_RendererID);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_RendererID);

}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
    Bind();
	vb.Bind();

    const auto& elements = layout.GetElements();
    unsigned int offset = 0;
	for(unsigned i = 0; i < elements.size(); i++)
    {
	    const auto& element = elements[i] ;
        //we need to ENABLE that vertex attribute!
        glEnableVertexAttribArray(i);

        //Telling GL our buffer layout. As long as the buffer is bound we can continue to this
        glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), reinterpret_cast<const void*>(offset));
        //index - first attribute
        //size, how many values
        //stride - amount of bites between each vertex
        //pointer - pointer INTO the actual attribute, bites into the beginning of the attribute.
        //There is a MACRO for finding the offset of something, no magic numbers >:(
        offset += element.count * VertexBufferElement::getSizeOfType(element.type);  
    }
}

void VertexArray::Bind() const
{
    glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const
{
    glBindVertexArray(0);
}
