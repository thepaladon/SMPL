#include "Bullet.h"
#include "Model.h"
#include "GameObject.h"
#include "Random.h"

#ifdef RASPBERRY
const std::string rootFilepath = "../../../";
#elif WIN32
const std::string rootFilepath("../");
#endif

Bullet::Bullet(Model* model, Camera* cam, Shader* shader, GameObject* owner, PlayerClass type)
	:type(type), owner(owner)
{
	obj = new GameObject(model, cam, shader, glm::vec3(0, -50.f, 0));
}

Bullet::~Bullet()
{
	delete obj;
}

void Bullet::Update(float dt)
{
	if (obj->getState()) {
		deltaTime = dt;
		totalTime += deltaTime;

		if (totalTime > timeUntilDespawn)
		{
			ResetBullet();
		}
	}
}


void Bullet::ResetBullet()
{
	obj->setState(false);
	obj->getTransComponent()->WorldPosition = { 0.f, -20.f, 0 };
	obj->setColliderPos(obj->getPhysicsComponent()->m_offset);
	obj->getPhysicsComponent()->getRigidBody()->setLinearVelocity({ 0.f, 0.f ,0.f });
	obj->getPhysicsComponent()->getRigidBody()->setTurnVelocity({ 0.f, 0.f ,0.f });
	obj->getPhysicsComponent()->getRigidBody()->setActivationState(ISLAND_SLEEPING);
}

void Bullet::Fire(glm::ivec2 dir)
{
	totalTime = 0;
	obj->getPhysicsComponent()->getRigidBody()->setActivationState(ACTIVE_TAG);

	obj->getTransComponent()->WorldPosition = owner->getTransComponent()->WorldPosition + glm::vec3(0.f, 1.f, 0.f);
	obj->getTransComponent()->Rotation = RotationSet(dir);
	obj->setColliderPos(obj->getPhysicsComponent()->m_offset);

	if(type == PlayerClass::Warrior) { obj->getPhysicsComponent()->getRigidBody()->applyTorqueImpulse({0.f, 10.f, 0.f}); }

	const btVector3 ownrVelocity = owner->getPhysicsComponent()->getRigidBody()->getLinearVelocity() / 2;
	const btVector3 velocityPower{ (power* dir.x) + ownrVelocity.getX() , 0.f, (power * dir.y) + ownrVelocity.getZ() };
	obj->getPhysicsComponent()->getRigidBody()->applyCentralImpulse(velocityPower);
}

glm::vec3 Bullet::RotationSet(glm::ivec2 dir)
{
	switch (type) {
	case PlayerClass::Mage:
		return RotationSetMage(dir);
		break;
	case PlayerClass::Valkyre:
		return RotationSetValkyre(dir);
		break;
	case PlayerClass::Warrior:
		return RotationSetWarrior(dir);
		break;
	case PlayerClass::Elf:
		return RotationSetElf(dir);
		break;
	default:
		return glm::vec3(0.f);
	}	
}

glm::vec3 Bullet::RotationSetMage(glm::ivec2 dir)
{
	glm::vec3 rotation{ 0.f, 0.f, 0.f };

	if (dir.y == 1) //up
	{
		if (dir.x == 1) //left
		{
			rotation = { 135.f, 90.f, 90.f };
		}
		else if (dir.x == 0) //none
		{
			rotation = { 90.f, 0.f, 90.f };
		}
		else if (dir.x == -1) //right
		{
			rotation = { 45.f, 90.f, 90.f };
		}
	}
	else if (dir.y == 0) //none
	{
		if (dir.x == 1) //left
		{
			rotation = { 180.f, 0.f, 90.f };

		}
		else if (dir.x == -1) //right
		{
			rotation = { 0.f, 0.f, 90.f };

		}
	}
	else if (dir.y == -1) //down
	{
		if (dir.x == 1) //left
		{
			rotation = { 225.f, 0.f, 90.f };


		}
		else if (dir.x == 0) //none
		{
			rotation = { -90.f, 0.f, 90.f };

		}
		else if (dir.x == -1) //right
		{
			rotation = { -45.f, 0.f, 90.f };
		}
	}

	return glm::radians(rotation);
}

glm::vec3 Bullet::RotationSetWarrior(glm::ivec2 dir)
{
	glm::vec3 rotation{ Rand(360.f), 90.f, 90.f };

	return glm::radians(rotation);
}

glm::vec3 Bullet::RotationSetValkyre(glm::ivec2 dir)
{
	glm::vec3 rotation{ 180.f, 0.f, 0.f };

	if (dir.y == 1) //up
	{
		if (dir.x == 1) //left
		{
			rotation = { -135.f, 0.f, 0.f };
		}
		else if (dir.x == 0) //none
		{
			rotation = { 180.f, 0.f, 0.f };
		}
		else if (dir.x == -1) //right
		{
			rotation = { 135.f, 0.f, 0.f };
		}
	}
	else if (dir.y == 0) //none
	{
		if (dir.x == 1) //left
		{
			rotation = { -90.f, 0.f, 0.f };
		}
		else if (dir.x == -1) //right
		{
			rotation = { 90.f, 0.f, 0.f };
		}
	}
	else if (dir.y == -1) //down
	{
		if (dir.x == 1) //left
		{
			rotation = { -45.f, 0.f, 0.f };
		}
		else if (dir.x == 0) //none
		{
			rotation = { 0.f, 0.f, 0.f };
		}
		else if (dir.x == -1) //right
		{
			rotation = { 45.f, 0.f, 0.f };
		}
	}

	return glm::radians(rotation);
}

glm::vec3 Bullet::RotationSetElf(glm::ivec2 dir)
{
	glm::vec3 rotation{ 0.f, 0.f, 0.f };

	if(dir.y == 1) //up
	{
		if (dir.x == 1) //left
		{
			rotation = { 45.f, 0.f, 0.f };

		}
		else if (dir.x == 0) //none
		{
			rotation = { 0.f, 0.f, 0.f };

		}
		else if (dir.x == -1) //right
		{
			rotation = { -45.f, 0.f, 0.f };
		}
	}
	else if(dir.y == 0) //none
	{
		if (dir.x == 1) //left
		{
			rotation = { 90.f, 0.f, 0.f };
		}
		else if (dir.x == -1) //right
		{
			rotation = { -90.f, 0.f, 0.f };
		}
	}
	else if(dir.y == -1) //down
	{
		if (dir.x == 1) //left
		{
			rotation = { 135.f, 0.f, 0.f };

		}
		else if (dir.x == 0) //none
		{
			rotation = { 180.f, 0.f, 0.f };
		}
		else if (dir.x == -1) //right
		{
			rotation = { -135.f, 0.f, 0.f };
		}
	}
	
	return glm::radians(rotation);
}


GunBulletManager::GunBulletManager(Camera* cam, Shader* shader, ObjectManager* manager, GameObject* owner, btDiscreteDynamicsWorld* world, PlayerClass role)
	:p_cam(cam), p_manager(manager), shader(shader), physicsWorld(world), owner(owner)
{

	switch (role)
	{
	//Load models needed for bullets
	case PlayerClass::Mage: 
		mageBullet = new Model(rootFilepath + "res/models/Crystal/crystal.obj");
		break;
	case PlayerClass::Valkyre:
		valkyreBullet = new Model(rootFilepath + "res/models/Dagger/Dagger.obj");
		break;
	case PlayerClass::Warrior:
		warriorBullet = new Model(rootFilepath + "res/models/VikingAxe/Viking Axe.obj");
		break;
	case PlayerClass::Elf:
		elfBullet = new Model(rootFilepath + "res/models/Arrow/arrow.obj");
		break;
	default: ;
	}

	createBullets(role);
}

GunBulletManager::~GunBulletManager()
{
	delete mageBullet;
	delete warriorBullet;
	delete elfBullet;
	delete valkyreBullet;
}

void GunBulletManager::Update(float dt)
{
	deltaTime = dt;
	timeUntilFire += deltaTime;

	for (auto bullet : bullets) { bullet->Update(dt); }
}

void GunBulletManager::createBullets(PlayerClass role)
{
	timeUntilFire += deltaTime;
	for (int i = 0; i < bulletPool; i++)
	{
		Bullet* bullet = nullptr;
		switch (role)
		{
		case PlayerClass::Mage:
			bullet = new Bullet(mageBullet, p_cam, shader, owner, role);
			bullet->getObj()->getTransComponent()->setPosition(glm::vec3(-1000.f));
			bullet->getObj()->getTransComponent()->setScale(glm::vec3(0.035f));
			bullet->getObj()->addPhysicsComponent(massOfBullet, glm::vec3(0.f, -1.f, 0.f), glm::vec3(1.f, 3.f, 1.f), physicsWorld, BULLET_TYPE, GROUND | ENEMY );
			break;

		case PlayerClass::Warrior:
			bullet = new Bullet(warriorBullet, p_cam, shader, owner, role);
			bullet->getObj()->getTransComponent()->setPosition(glm::vec3(-100.f));
			bullet->getObj()->getTransComponent()->setScale(glm::vec3(0.01f));
			bullet->getObj()->addPhysicsComponent(massOfBullet, glm::vec3(-0.2f, 0.f, 0.f), glm::vec3(0.995f, 3.385f, 0.322), physicsWorld, BULLET_TYPE, GROUND | ENEMY );
			break;

		case PlayerClass::Valkyre:
			bullet = new Bullet(valkyreBullet, p_cam, shader, owner, role);
			bullet->getObj()->getTransComponent()->setPosition(glm::vec3(-100.f));
			bullet->getObj()->getTransComponent()->setScale(glm::vec3(0.1f));
			bullet->getObj()->addPhysicsComponent(massOfBullet, glm::vec3(0.f, 0.f, 1.8f), glm::vec3(.675f, .375f, 6.125f), physicsWorld, BULLET_TYPE, GROUND | ENEMY );
			break;

		case PlayerClass::Elf:
			bullet = new Bullet(elfBullet, p_cam, shader, owner, role);
			bullet->getObj()->getTransComponent()->setScale(glm::vec3(0.09f, 0.09f, 0.07f));
			bullet->getObj()->addPhysicsComponent(massOfBullet, glm::vec3(0.f, 0.f, -2.5f), glm::vec3(0.40f, 0.40f, 4.75f), physicsWorld, BULLET_TYPE, GROUND | ENEMY );
			break;
		}

		bullet->getObj()->setType(BULLET_TYPE);
		bullet->getObj()->getTransComponent()->setPosition(glm::vec3(0.f, -1000.f, 0.f));
		bullet->getObj()->setColliderPos(bullet->getObj()->getPhysicsComponent()->m_offset);
		bullet->getObj()->getPhysicsComponent()->getRigidBody()->setActivationState(ISLAND_SLEEPING);
		bullet->getObj()->getTransComponent()->UpdateModelMatrix();
		bullets.push_back(bullet);
		p_manager->addObject("bullet" + std::to_string(i), bullet->getObj());
	}
}

void GunBulletManager::FireBullet(glm::ivec2 dir)
{
	if(timeUntilFire > timeBetween)
	{
		timeUntilFire = 0;

		bullets[numBullets]->getObj()->setState(true);
		bullets[numBullets]->Fire(dir);
		bullets[(numBullets + 2) % bulletPool]->ResetBullet();
		numBullets++;

		if (numBullets >= bulletPool) { numBullets = 0; }
	}
}
