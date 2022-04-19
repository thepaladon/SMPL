#include "Mesh.h"

#include "IndexBuffer.h"
#include "Renderer.h"
#include "VerterBufferLayout.h"
#include "VertexArray.h"

#ifdef RASPBERRY
#include "external/stb_image/stb_image.h"
#elif WIN32
#include "stb_image/stb_image.h"
#endif

Mesh::Mesh(std::string name, const void* vertData, unsigned vertSize, const unsigned* indData, unsigned indxSize)
{
	if (name == "vertex") {
		usesIndexBuffer = false;
		//vertLines = (int)indData;
		m_vao = new VertexArray();
		m_vb = new VertexBuffer(vertData, vertSize);
		m_ib = new IndexBuffer(0, 0);
		m_layout = new VertexBufferLayout();
		
		m_layout->PushFloat(3);
		m_layout->PushFloat(3);
		m_layout->PushFloat(2);
		//m_layout->PushFloat(3);
		//m_layout->PushFloat(3);

		m_vao->AddBuffer(*m_vb, *m_layout);

	}
	else if (name == "index") {
		usesIndexBuffer = true;
		m_vao = new VertexArray();
		m_vb = new VertexBuffer(vertData, vertSize);
		m_ib = new IndexBuffer(indData, indxSize); //not used here
		m_layout = new VertexBufferLayout();

		m_layout->PushFloat(3);
		m_layout->PushFloat(3);
		m_layout->PushFloat(2);
		m_layout->PushFloat(3);
		m_layout->PushFloat(3);

		m_vao->AddBuffer(*m_vb, *m_layout);
	}

	else if (name == "debug") {
		usesIndexBuffer = false;
		m_vao = new VertexArray();
		m_vb = new VertexBuffer(vertData, vertSize);
		m_ib = new IndexBuffer(0, 0); //not used here
		m_layout = new VertexBufferLayout();

		m_layout->PushFloat(3);
		m_layout->PushFloat(3);

		m_vao->AddBuffer(*m_vb, *m_layout);
	}
	else
	{
		printf("ERROR: No mesh created");
	}
}


Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned>& indices, std::vector<Texture>& textures)
{
	m_vertices = vertices;
	m_indices = indices;
	m_textures = textures;
	
	setupMesh(vertices, indices);
}

Mesh::~Mesh()
{
	delete m_vao;
	delete m_vb;
	delete m_ib;
	delete m_layout;
}

void Mesh::UpdateMesh(const void* vertData, unsigned vertSize)
{
	delete m_vao;
	delete m_vb;

	m_vao = new VertexArray();
	m_vb = new VertexBuffer(vertData, vertSize);
	
	m_vao->AddBuffer(*m_vb, *m_layout);
}


void Mesh::setupMesh(std::vector<Vertex>& vertices, std::vector<unsigned>& indices)
{
	usesIndexBuffer = true;
	m_vao = new VertexArray();
	m_vb = new VertexBuffer(vertices.data(), static_cast<unsigned>(vertices.size()) * sizeof(vertices[0]));
	m_ib = new IndexBuffer(indices.data(), static_cast<unsigned>(indices.size()));
	m_layout = new VertexBufferLayout();

	m_layout->PushFloat(3); //position
	m_layout->PushFloat(3); //normal
	m_layout->PushFloat(2); //texture
	m_layout->PushFloat(3); //tangent
	m_layout->PushFloat(3); //bitangent

	m_vao->AddBuffer(*m_vb, *m_layout);
}





