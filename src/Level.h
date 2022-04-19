#pragma once
#include <vector>
#include "GameState.h"


class btDiscreteDynamicsWorld;
class ObjectManager;
class Renderer;
class EnemyManager;
class Camera;
class Shader;
class LightManager;
class Map;
class Player;
class GunBulletManager;

class Level : public GameState
{
public:
	Level(Renderer* renderer, Camera* cam, Shader* shader, UI* ui, btDiscreteDynamicsWorld* physicsWorld, PhysicsManager* physicsmanager, bool keyState[65536]);
	~Level();

	void LoadLevel();


	void Update(float dt) override;
	void UpdateImGui() override;
	void Render() override;
	void HandleEvents() override;
	void Input();

	void RestartLevel();

	Map* getMap() { return map; }
	ObjectManager* getObjManager() { return objectManager; }

private:
	float time = 0.f;
	float pauseTimer = 0.5f;
	bool firstBuild = true;
	bool paused = false;
	float fogFar = 4.f, fogNear = 2.f;
	Map* map;
	LightManager* lights;
	std::vector<Player*> players;
	std::vector<GunBulletManager*> guns;
	EnemyManager* enemies;
	ObjectManager* objectManager;

	//References
	bool* keyState; //keys
	UI* ui;
	btDiscreteDynamicsWorld* physicsWorld;
	PhysicsManager* p_physicsManager;
	Renderer* renderer;
	Camera* cam;
	Shader* shader;


};
