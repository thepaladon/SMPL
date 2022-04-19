#pragma once
#include <string>
#include <vector>
#include <bullet/LinearMath/btVector3.h>
#include "Shader.h"

class btDiscreteDynamicsWorld;
class ObjectManager;
class Camera;
class Model;
class GameObject;

enum class PlayerClass
{
	Mage = 0,
	Valkyre = 1,
	Warrior = 2,
	Elf = 3
};

enum BulletDir
{
	UP = 1,
	VERTICAL_NONE = 0,
	DOWN = -1,
	LEFT = 1,
	HORIZONTAL_NONE = 0,
	RIGHT = -1
};

class Bullet
{
public:
	Bullet(Model* model, Camera* cam, Shader* shader, GameObject* owner, PlayerClass type);
	~Bullet();

	void Update(float dt);
	void Fire(glm::ivec2 dir);
	void ResetBullet();
	GameObject* getObj() { return obj; }
	GameObject* getOwner() { return owner; }

	glm::vec3 RotationSet(glm::ivec2 dir);
	glm::vec3 RotationSetMage(glm::ivec2 dir);
	glm::vec3 RotationSetWarrior(glm::ivec2 dir);
	glm::vec3 RotationSetValkyre(glm::ivec2 dir);
	glm::vec3 RotationSetElf(glm::ivec2 dir);


private:
	float deltaTime;
	float totalTime = 0;
	const float timeUntilDespawn = 5.f; //seconds
	const float power = 25.f;
	GameObject* owner;
	GameObject* obj;
	PlayerClass type;
};

class GunBulletManager
{
public:
	GunBulletManager(Camera* cam, Shader* shader, ObjectManager* manager, GameObject* owner, btDiscreteDynamicsWorld* world, PlayerClass role);
	~GunBulletManager();

	void Update(float dt);
	void createBullets(PlayerClass role);
	void FireBullet(glm::ivec2 dir);

private:
	//TIMER
	float deltaTime;
	float timeUntilFire = 0;
	float timeBetween = 0.4f; //ms between shots

	//bullet management
	int numBullets = 0;
	const int bulletPool = 10;
	const float massOfBullet = .5f;
	std::vector<Bullet*> bullets;

	//references
	Camera* p_cam;
	Shader* shader;
	ObjectManager* p_manager;
	btDiscreteDynamicsWorld* physicsWorld;
	GameObject* owner;

	//models of bullets
	Model* mageBullet;
	Model* warriorBullet;
	Model* elfBullet;
	Model* valkyreBullet;
	
};
