#include "Player.h"
#include "GameObject.h"

void Player::PlayerLogicUpdate(float deltaTime)
{
	playerSpeedDT = deltaTime * playerSpeed;
	p_playerObj->getPhysicsComponent()->LimitVelocity(playerMaxVelocity);
	
}

void Player::moveForward(Direction dir)
{
	btVector3 velocity{p_cam->getCamLookAt().x, 0.f, p_cam->getCamLookAt().z};
	velocity = velocity * static_cast<btScalar>(dir) * playerSpeed * 0.5f;
	p_playerObj->getPhysicsComponent()->getRigidBody()->activate(true);
	p_playerObj->getPhysicsComponent()->getRigidBody()->applyCentralImpulse(velocity);
}

void Player::moveSideways(Direction dir)
{
	const glm::vec3 cameraRight = normalize(cross(glm::vec3(0.f, 1.f, 0.f), p_cam->getCamLookAt()));
	btVector3 velocity{ cameraRight.x, 0.f, cameraRight.z };
	velocity = velocity * static_cast<btScalar>(dir) * playerSpeed * 4.f;
	p_playerObj->getPhysicsComponent()->getRigidBody()->activate(true);
	p_playerObj->getPhysicsComponent()->getRigidBody()->applyCentralImpulse(velocity);
}

void Player::jump(btScalar jumpScale)
{
	p_playerObj->getPhysicsComponent()->getRigidBody()->activate(true);
	p_playerObj->getPhysicsComponent()->getRigidBody()->applyCentralImpulse({0.f, jumpScale, 0.f});
}
