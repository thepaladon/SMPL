#pragma once
#include <string>
#include <glm/vec3.hpp>
#include <LinearMath/btScalar.h>

#include "GameObject.h"

enum Direction;
class GunBulletManager;
class Camera;
class GameObject;

class Player 
{
public:
	Player(Camera* cam): p_cam(cam) {};
	~Player() = default;

	void setPlayerObj(GameObject* obj, btDiscreteDynamicsWorld* physicsWorld)
	{
		p_playerObj = obj;
		p_playerObj->addPhysicsComponent(1.f, glm::vec3(0.f), glm::vec3(2.f), physicsWorld, PLAYER, ALL);
		p_playerObj->setType(PLAYER);
		p_playerObj->setHealthObject(&health, &maxHealth);
		p_playerObj->getPhysicsComponent()->getRigidBody()->setFriction(0.f);
		p_playerObj->getPhysicsComponent()->getRigidBody()->setAngularFactor({0.f, 1.f, 0.f});
	}

	void PlayerLogicUpdate(float deltaTime);
	void moveForward(Direction dir);
	void moveSideways(Direction dir);
	void jump(btScalar jumpScale);

	GameObject* getObject() const { return p_playerObj; }

	void restoreHealth() { health = maxHealth; }
	int getHealth()  { return health; }
	int* getHealthP()  { return &health; }
	int* getMaxHealthP()  { return &maxHealth; }

private:
	int health = 300;
	int maxHealth = 300;
	float playerSpeed = .1f;
	float playerSpeedDT;
	float playerMaxVelocity = 15.f;
	btVector3 m_velocity;

	GameObject* p_playerObj;
	Camera* p_cam;
};