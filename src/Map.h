#pragma once
#include <string>
#include <vector>
#include <glm/vec2.hpp>

class ObjectManager;
class EnemyManager;
class Player;
class btDiscreteDynamicsWorld;
class Camera;
class Shader;
class GameObject;
class Renderer;
class Mesh;
class Model;

class Map
{
public:
	Map(Renderer* rend, Camera* cam, Shader* shader, btDiscreteDynamicsWorld* physicsWorld, EnemyManager* manager, ObjectManager* p_objManager);
	~Map();

	void setPlayer(Player* player) { p_player = player; }
	void CSVparser(FILE* file, int p_map[100][100]);

	void ReBuildLevel(std::string CSVpath, float offset, std::string wallFilepath, std::string groundFilepath);
	void BuildLevel(std::string CSVpath, float offset, std::string wallFilepath,  std::string groundFilepath);

	void Draw();

	float distanceCull = 20.f;
private:
	bool flipValue;
	int map[100][100];
	const float m_offset = 4.f;

	Model* m_ground;
	Model* m_walls;
	std::vector<GameObject*> level;

	//References
	Camera* p_cam;
	Shader* p_shader;
	Renderer* p_renderer;
	Player* p_player;
	ObjectManager* p_objManager;
	EnemyManager* p_managerEnemies;
	btDiscreteDynamicsWorld* physicsWorld;
};
