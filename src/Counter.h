#pragma once
#include <vector>

class Counter
{
public:
	static Counter& GetCounter()
	{
		return s_Instance;
	}

	void addShaderID(unsigned id) { shaderIDs.push_back(id); }
	void addTextureID(unsigned id) { textureIDs.push_back(id); }
	void addVertexArrayID(unsigned id) { vertexArrayID.push_back(id); }
	void addVertexBufferID(unsigned id) { vertexBufferID.push_back(id); }
	void addIndexID(unsigned id) { indexID.push_back(id); }

	void clearAll()
	{
		indexID.clear();
		shaderIDs.clear();
		textureIDs.clear();
		vertexArrayID.clear();
		vertexBufferID.clear();
	}

	std::vector<unsigned> shaderIDs;
	std::vector<unsigned> textureIDs;
	std::vector<unsigned> vertexArrayID;
	std::vector<unsigned> vertexBufferID;
	std::vector<unsigned> indexID;

private:
	Counter() {};
	static Counter s_Instance;

};

