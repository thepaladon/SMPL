#include "Enemy.h"
#include "GameObject.h"
#include "Player.h"
#include <glm/gtx/vector_angle.hpp>

#ifdef RASPBERRY
const std::string rootFilepath = "../../../";
#elif WIN32
const std::string rootFilepath("../");
#endif


Enemy::Enemy(GameObject* obj)
{
	m_object = obj;
}

Enemy::~Enemy()
{
	delete m_object;
}

void Enemy::UpdateEnemy()
{
	if (m_object->getState()) {
		distanceToPlayer = glm::distance(p_player->getObject()->getTransComponent()->WorldPosition, m_object->getTransComponent()->WorldPosition);
		if (distanceToPlayer < 25) {
			m_object->getPhysicsComponent()->getRigidBody()->activate();
			const glm::vec3 dist = glm::normalize(p_player->getObject()->getTransComponent()->WorldPosition - m_object->getTransComponent()->WorldPosition);
			const btVector3 force = { dist.x, 0.f, dist.z };
			m_object->setColliderRotation(m_object->getTransComponent()->Rotation);
			m_object->getPhysicsComponent()->getRigidBody()->applyCentralImpulse(force);
			m_object->getPhysicsComponent()->LimitVelocity(enemySpeed);
		}
	}

	//code dump: Rotation
	//m_object->getTransComponent()->Rotation.x = glm::orientedAngle(p_player->getObject()->getTransComponent()->WorldPosition, m_object->getTransComponent()->WorldPosition, glm::vec3(1.f, 0.f, 0.f));
	//angleBetweenPoints(p_player->getObject()->getTransComponent()->WorldPosition, m_object->getTransComponent()->WorldPosition);
}

float Enemy::angleBetweenPoints(glm::vec3 a, glm::vec3 b) const
{
	float deltaY = abs(b.z - a.z);
	float deltaX = abs(b.x - a.x);
	return atan2(deltaY, deltaX);
}

EnemyManager::EnemyManager(Player* player, Camera* cam, Shader* shader, ObjectManager* manager, btDiscreteDynamicsWorld* world)
	:p_cam(cam), p_shader(shader), p_manager(manager), p_physicsWorld(world), p_player(player)
{
	for(int i = 0; i < NUM_ENEMIES; i++)
	{
		enemyModels.push_back(nullptr);
	}
}

EnemyManager::~EnemyManager()
{
	for(auto model : enemyModels)
	{
		delete model;
	}
}

void EnemyManager::Update()
{
	for(auto enemy : enemies)
	{
		enemy->UpdateEnemy();
	}
}

void EnemyManager::createEnemy(TypeOfEnemy type, glm::vec3 position)
{
	GameObject* enemyObj = nullptr;
	switch(type) // Violation of YAGNI but it is intentional
	{
	case BASIC:

		if(enemyModels[type] == nullptr){
			enemyModels[type] = new Model(rootFilepath + "res/models/goblin/goblin.obj");
		}

		enemyObj = new GameObject(enemyModels[type], p_cam, p_shader, position, glm::vec3(0), glm::vec3(0.1, 0.1, 0.1));
		enemyObj->addPhysicsComponent(1.f, 
			glm::vec3(0.f, -3.750, 0.f), 
			glm::vec3(2.f, 5.25, 2.f), 
			p_physicsWorld, 
	ENEMY, GROUND | BULLET_TYPE | PLAYER );
		enemyObj->getPhysicsComponent()->getRigidBody()->setAngularFactor({ 0.f, 1.f, 0.f });
		break;
	}

	enemyObj->setType(ENEMY);
	p_manager->addObject("ENEMY" + std::to_string(enemyID), enemyObj);
	enemies.push_back(new Enemy(enemyObj));
	enemies[enemyID]->setPlayer(p_player);
	enemyID++;	
}

void EnemyManager::deleteEnemies()
{
	for (auto enemy : enemies) {
		enemy->getObj()->removePhysicsComponent(p_physicsWorld);
		enemy = nullptr;
	}

	p_manager->removeEnemies();

	for (auto enemy : enemies) {
		delete enemy;
	}

	enemies.clear();
	enemyID = 0;
}
