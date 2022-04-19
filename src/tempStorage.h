#pragma once

namespace TempStorage {

	constexpr size_t qX = 50;
	constexpr size_t qY = 50;
	constexpr size_t MaxQuadCount = qX * qY;
	constexpr size_t MaxVertexCount = MaxQuadCount * 4;
	constexpr size_t MaxIndexCount = MaxQuadCount * 6;

	struct Vertex
	{
		glm::vec3 position = {0.f, 0.f, 0.f};
		glm::vec2 texture = {0.f, 0.f};
	};

	static std::array<Vertex, 4> CreateQuad(float x, float y)
	{
		float size = 1.f;

		Vertex v0;
		v0.position = { x, 0.f, y };
		v0.texture = { 0.0f, 0.0f, };

		Vertex v1;
		v1.position = { x + size, 0.f, y };
		v1.texture = { 1.0f, 0.0f, };

		Vertex v2;
		v2.position = { x + size, 0.f, y + size };
		v2.texture = { 1.0f, 1.0f, };

		Vertex v3;
		v3.position = { x, 0.f, y + size };
		v3.texture = { 0.0f, 1.0f, };

		return { v0, v1, v2, v3 };
	}

	//3 vertex pos, 2 dogTex coords, 3*2 triangle cubeVerts, 6 sides of cube
	float cubeVerts[8 * 6 * 6] = {

	// positions // normals // texture coords
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
	0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

	-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

	0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};

	float planeVertsNOIND[30] = {
	-0.5f, 0.0f, 0.5f,  0.0f, 0.0f,
	0.5f, 0.0f, 0.5f,   1.0f, 0.0f,
	-0.5f, 0.0f, -0.5f, 0.0f, 1.0f,

	0.5f, 0.0f, 0.5f,   1.0f, 0.0f,
	0.5f, 0.0f, -0.5f,    1.0f, 1.0f,
	-0.5f, 0.0f, -0.5f, 0.0f, 1.0f,
	};

	float planeVerts[20] = {
		-0.5f, 0.0f, -0.5f, 0.0, 0.0,
		0.5f, 0.0f, -0.5f, 1.0, 0.0,
		0.5f, 0.0f, 0.5f, 1.0, 1.0,
		-0.5f, 0.0f, 0.5f, 0.0, 1.0
	};

	const int unsigned planeIndex[6] = {
	0, 1, 2,
	2, 3, 0, 
	};

	float planeVertsBATCH[40] = {
	-1.5f, 0.0f, -0.5f, 0.0, 0.0,
	-0.5f, 0.0f, -0.5f, 1.0, 0.0,
	-0.5f, 0.0f, 0.5f, 1.0, 1.0,
	-1.5f, 0.0f, 0.5f, 0.0, 1.0,

	0.5f, 0.0f, -0.5f, 0.0, 0.0,
	1.5f, 0.0f, -0.5f, 1.0, 0.0,
	1.5f, 0.0f, 0.5f, 1.0, 1.0,
	0.5f, 0.0f, 0.5f, 0.0, 1.0
	};

	const int unsigned planeIndexBATCH[12] = {
	0, 1, 2 , 2, 3, 0,
	4, 5, 6 , 6, 7, 4,
	};


}