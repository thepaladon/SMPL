#pragma once
#include <string>
#include "IndexBuffer.h"
#include "VerterBufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 BiTangent;
};

class Mesh
{
public:
	Mesh(std::string name, const void* vertData, unsigned vertSize, const unsigned* indData, unsigned indxSize);
	Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures);
	~Mesh();

	void setupMesh(std::vector<Vertex>& vertices, std::vector<unsigned>& indices);

	void UpdateMesh(const void* vertData, unsigned vertSize);

	void setVertLines(int lines) { vertLines = lines; }
	unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma);
	void addTexture(Texture tex) { m_textures.push_back(tex); }
	bool setKeepVerticesInCPU();

	bool staticBuff{};
	bool usesIndexBuffer;
	unsigned vertLines{};

	
	std::vector<Texture>    m_textures;
	std::vector<Vertex>     m_vertices;
	std::vector<unsigned>	m_indices;

	std::string name = "CUBE"; //later set this to the OBJ file you parse to get the information for this class
	VertexArray* m_vao;
	VertexBuffer* m_vb;
	IndexBuffer* m_ib;
private:
	VertexBufferLayout* m_layout;

	
};
