#pragma once


#ifdef RASPBERRY
#include <array>
#include <EGL/egl.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#elif WIN32
#include <array>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

#endif

#define ARRAY_SIZE_X 1
#define ARRAY_SIZE_Y 1
#include <string>
#include <vector>

class UI;
class PhysicsManager;
class GameStateManager;
class EnemyManager;
class Map;
class GunBulletManager;
class LightManager;
class ObjectManager;
class btDiscreteDynamicsWorld;
class Model;
class Renderer;
class GameObject;
class Mesh;
class Texture;
class Shader;
class VertexBufferLayout;
class IndexBuffer;
class VertexBuffer;
class VertexArray;
class Camera;
class Input;
class Player;
class Level;

enum class MouseButton
{
	Left = 0,
	Right = 1
};

#ifdef RASPBERRY
const std::string rootFilepath = "../../../";
#elif WIN32
const std::string rootFilepath("../");
#endif


class Name
{
public:
	Name();
	~Name() = default;

	void GenerateNewName();
	std::string getFullName() const { return name + " " + surname; }

private:
	int linesFile1 = 0;
	int linesFile2 = 0;
	std::string name;
	std::string surname;
};

class Game
{
public:
	Game();
	~Game() = default;

	void passWindowSize(int x, int y) { win_width = x; win_height = y; }
	void passPhysicsWorld(btDiscreteDynamicsWorld* world, PhysicsManager* manager)
	{ p_physicsWorld = world; p_physicsmanager = manager; }

	void passDeltaTime (float dt) { deltaTimeR = dt; }
	void OnInit();
	void Render();
	void Update(float deltaTime);
	void OnImGuiRender(float deltaTime, unsigned frames, float totalTime);
	void OnShutdown();

	Camera* getCamera() const;

	void Input();

#ifdef RASPBERRY
	void OnKeyUp();
	void KeyEvent(KeySym key, bool pressed);
	void MouseButtonEvent(MouseButton key, int pressed);
	void MouseMove(int mousexRel, int mouseyRel);
#endif

	float deltaTimeR;
	bool gameRunning = true;

	// Input
	bool keyState[65536]{};   
	bool mouseState[5]{};
	int mouseX{ 0 };
	int mouseY{ 0 };

private:
	btDiscreteDynamicsWorld* p_physicsWorld;
	PhysicsManager* p_physicsmanager;
	
	int win_width = 0;
	int win_height = 0;

	float FPS;
	int typeOfRendering = 0;
	int counterDTA = 0;
	;
	std::array<float, 60> deltaTimeArray;

	UI* ui;
	Renderer* renderer;
	Camera * camera;
	Shader* lightShader;
	GameStateManager* level;

	std::vector<Name*> names;

	glm::vec4 backgroundColor{ 0.1, 0.1, 0.1, 1.0 };
	glm::vec4 shaderColor{ 1.0, 1.0, 1.0, 1.0 };

	float positions[16] = { // vec2 position, vec2 dogTex coords,
	   -0.5f, -0.5f, 0.0, 0.0,     //0 index
	   0.5f, -0.5f,  1.0, 0.0,     //1 index
	   0.5f, 0.5f,	1.0, 1.0,     //2 index
	   -0.5f, 0.5f,  0.0, 1.0,     //3 index
	};

	unsigned indices[6] = {
	 0, 1, 2, //we draw 1 triangle using indices
	 2, 3, 0, //we draw 1 triangle using indices
	};

	int fogMode[3] = { 0x0800, 0x0801, 0x2601 };// Storage For Three Types Of Fog
	int fogfilter = 2; // Which Fog To Use
	glm::vec4 fogColor = { 0.5f, 0.5f, 0.5f, 1.0f };
	float density = 0.35f;
	float fogStart = 1.0f;
	float fogEnd = 5.0f;
	float dist = 5.0f;
};

bool GLLogCall();

