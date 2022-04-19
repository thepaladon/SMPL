#pragma once
#include <string>
#include <glm/glm.hpp>

#include "Model.h"
#include "PhysicsComponent.h"
#include "Transform.h"

class Model;
class Shader;
class Texture;
class Transform;
class Camera;
class Mesh;

class GameObject
{
public:
	GameObject(std::string filepath, Camera* cam, Shader* shader, glm::vec3 pos = glm::vec3(0), glm::vec3 rotation = glm::vec3(0), glm::vec3 scale = glm::vec3(1.f));
	GameObject(Model* information, Camera* cam, Shader* shader, glm::vec3 pos = glm::vec3(0), glm::vec3 rotation = glm::vec3(0), glm::vec3 scale = glm::vec3(1.f));
	GameObject(Mesh* information, Camera* cam, Shader* shader, glm::vec3 pos = glm::vec3(0), glm::vec3 rotation = glm::vec3(0), glm::vec3 scale = glm::vec3(1.f));
	~GameObject();
	
	void Update();
	void PassUniforms();
	void UpdateColor(glm::vec3 color);
	void setShader(Shader* shader);
	void setHealthObject(int* phealth, int* pmaxHealth) { health = phealth, healthMax = pmaxHealth; };

	void addModelComponent(std::string filepath) { m_model = new Model(filepath); }
	void addModelComponent(Mesh* information) { m_model = new Model(information); }
	void addModelComponent(Model* information) { m_model = information; }

	void addTransComponent(glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale) { m_transform = new Transform(pos, rotation, scale); }
	void addTransComponent() { m_transform = new Transform(); }

	void addPhysicsComponent(float mass, glm::vec3 offset, glm::vec3 scale, btDynamicsWorld* world, short aGroup, short aMask) { m_physicsBody = new PhysicsComponent(mass, m_transform->WorldPosition, offset, scale, world, aGroup, aMask); }
	void addPhysicsComponent(float mass, glm::vec3 offset, btDynamicsWorld* world) { m_physicsBody = new PhysicsComponent(mass, m_transform->WorldPosition, offset, world, m_model); }

	void removePhysicsComponent(btDiscreteDynamicsWorld* world) {
		if (m_physicsBody != nullptr)
		{
			world->removeRigidBody(m_physicsBody->getRigidBody());
			delete m_physicsBody->getRigidBody()->getCollisionShape();
			delete m_physicsBody->getRigidBody()->getMotionState();
			delete m_physicsBody->getRigidBody();
		}
	}

	void setColliderPos(glm::vec3 offset);
	void setColliderRotation(glm::vec3 rotation);
	void setColliderScale(glm::vec3 scale);
	void setActiveObject(bool state) { activeObject = state; }

	void OnCollisionExecute() {};
	void OnCollisionExitExecute() {};
	void PlayerCollision(GameObject* collided);
	void EnemyCollision(GameObject* collided);
	void BulletCollision(GameObject* collided);

	void setState(bool newState) { activeObject = newState; }
	void setType(ObjectTypes passtype) { type = passtype; }

	//GETTERS
	bool getState() const { return activeObject; }
	bool* getStateP() { return &activeObject; }
	ObjectTypes getType() const { return type; }
	Model* getModelComponent() const { return m_model; }
	Transform* getTransComponent() const { return m_transform; }
	Shader* getShader() const { return p_shader; }
	PhysicsComponent* getPhysicsComponent() const { if (m_physicsBody != nullptr) return m_physicsBody; return nullptr; }
	
private:
	int* health;
	int* healthMax;
	bool activeObject = true;
	float materialShininess = 16.f;
	ObjectTypes type{NONE}; 

	//Components
	Shader* p_shader{nullptr};
	Camera* p_camera { nullptr };
	PhysicsComponent* m_physicsBody{ nullptr };
	Model* m_model{ nullptr };
	Transform* m_transform{ nullptr };
};