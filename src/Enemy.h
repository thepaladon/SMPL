#pragma once
#include <vector>
#include <glm/vec3.hpp>

class Model;
class Player;
class btDiscreteDynamicsWorld;
class ObjectManager;
class Shader;
class Camera;
class GameObject;

enum TypeOfEnemy
{
	BASIC = 0,
	NUM_ENEMIES
};

class Enemy
{
public:
	Enemy(GameObject* obj);
	~Enemy();

	void setPlayer(Player* player) { p_player = player; }
	void UpdateEnemy();
	GameObject* getObj() { return m_object; }
	float angleBetweenPoints(glm::vec3 a,glm::vec3 b) const;

private:

	bool activated;
	float distanceToPlayer = 0;
	float enemySpeed = 10.f;
	TypeOfEnemy type;
	GameObject* m_object;
	Player* p_player;
};

class EnemyManager
{
public:
	EnemyManager(Player* player, Camera* cam, Shader* shader, ObjectManager* manager, btDiscreteDynamicsWorld* world);
	~EnemyManager();

	void Update();
	void createEnemy(TypeOfEnemy type, glm::vec3 position);
	void deleteEnemies();
private:

	int enemyID = 0;
	std::vector<Enemy*> enemies;
	std::vector<Model*> enemyModels;

	//references
	ObjectManager* p_manager;
	btDiscreteDynamicsWorld* p_physicsWorld;
	Camera* p_cam;
	Shader* p_shader;
	Player* p_player;

};
