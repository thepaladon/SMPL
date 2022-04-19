#pragma once

#ifdef RASPBERRY
#include <GLES3/gl3.h>
#elif WIN32
#include <GL/glew.h>
#endif

#include <glm/vec4.hpp>
#include "Mesh.h"

class GameObject;
class Model;

class Renderer
{
public:
	Renderer(): drawCallCounter(0), bgColor(glm::vec4(0.f)), typeOfRend(triangles), wireframeMode(false) {} ;
	~Renderer() = default;

	void Clear(Shader* shader) const;
	void Draw(GameObject* obj, Shader* shader);
	void Draw(Mesh* mesh, Shader* shader);
	void Draw(Model* model, Shader* shader);

	void DrawMap();
	void ImGuiLogic();

	void setBgColor(glm::vec4 newCol) { bgColor = newCol; }

	void resetCounter() { drawCallCounter = 0; }
	unsigned getCounter() { return drawCallCounter; }

private:
	enum typeOfRendering
	{
		triangles = GL_TRIANGLES,
		lines = GL_LINES,

	};

	int renderMode = 0;
	glm::vec4 bgColor{ 0.1, 0.1, 0.1, 1.0 };
	bool wireframeMode = false;
	typeOfRendering typeOfRend;
	unsigned drawCallCounter;
};
