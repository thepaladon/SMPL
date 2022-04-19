#include "GameObject.h"

#ifdef RASPBERRY
#include <GLES3/gl3.h>
#include "external/imgui/imgui.h"
#elif WIN32
#include <GL/glew.h>
#include "imgui/imgui.h"
#endif


#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "Camera.h"
#include "Mesh.h"
#include "Model.h"
#include "Texture.h"
#include "Transform.h"

inline float Rand(float range) { return (static_cast<float>(rand()) / RAND_MAX) * range; }

GameObject::GameObject(std::string filepath, Camera* cam, Shader* shader, glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale)
	:p_camera(cam), p_shader(shader)
{
	addModelComponent(filepath);
	addTransComponent(pos, rotation, scale);
}

GameObject::GameObject(Model* information, Camera* cam, Shader* shader, glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale)
	:m_model(information), p_camera(cam), p_shader(shader)
{
	addTransComponent(pos, rotation, scale);
}

GameObject::GameObject(Mesh* information, Camera* cam, Shader* shader, glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale)
	: p_camera(cam), p_shader(shader)
{
	addModelComponent(information);
	addTransComponent(pos, rotation, scale);
}

GameObject::~GameObject()
{
	if (m_transform != nullptr) {
		delete m_transform;
		m_transform = nullptr;
	}

	if (m_model != nullptr) {

		m_model = nullptr;
	}

	if (m_physicsBody != nullptr) {
		delete m_physicsBody;
		m_physicsBody = nullptr;
	}
}

void GameObject::Update()
{

	//RigidBody Determines the Model Matrix ? That might be bad.
	if (m_physicsBody != nullptr)
	{
		if (m_physicsBody->getRigidBody()->isActive()) {
		btMotionState* state = m_physicsBody->getRigidBody()->getMotionState();
		btTransform transform;
		state->getWorldTransform(transform);

		glm::mat4 model = glm::mat4(1.f);
		transform.getOpenGLMatrix(&model[0][0]);
		model = glm::translate(model, m_physicsBody->getOffset());
		glm::mat4 scale = glm::mat4(1.f);
		scale = glm::scale(scale, m_transform->Scale);
		model = model * scale; 
		m_physicsBody->getRigidBody()->getCollisionShape()->setLocalScaling({ 1.f, 1.f, 1.f });
		glm::quat rotation;
		glm::vec3 skew; glm::vec4 perspective; //not used. 
		decompose(model, m_transform->Scale, rotation, m_transform->WorldPosition, skew, perspective);
		rotation = glm::conjugate(rotation);
		m_transform->Rotation = glm::eulerAngles(rotation) * 3.14159f / 180.f;
		m_transform->Model = model;
		}
	}
	else if(activeObject)
	{
		m_transform->UpdateModelMatrix();
	}

	PassUniforms();
}



void GameObject::PassUniforms()
{
	p_shader->Bind();
	glm::mat4 MVP = p_camera->getProjectionMatrix() * p_camera->getViewMatrix() * m_transform->Model;
	p_shader->SetUniform1f("material.shininess", materialShininess);
	p_shader->SetUniformMat4f("model", m_transform->Model);
	p_shader->SetUniformMat4f("u_MVP", MVP);
}

void GameObject::UpdateColor(glm::vec3 color)
{
	for (int i = 0; i < m_model->getSizeOfMeshes(); i++)
	{
		m_model->getMeshes(i)->m_textures[0].UpdateTexture(color.r * 255, color.g * 255, color.b * 255, 255);
	}
}

void GameObject::setShader(Shader* shader)
{
	p_shader = shader;
}

void GameObject::setColliderPos(glm::vec3 offset)
{
	m_physicsBody->getRigidBody()->activate(true);
	glm::vec3 newPos = m_transform->WorldPosition - offset;
	btTransform transform;

	transform.setIdentity();
	transform.setOrigin({ newPos.x, newPos.y, newPos.z });
	transform.setRotation({ m_transform->Rotation.x, m_transform->Rotation.y , m_transform->Rotation.z });
	btDefaultMotionState* newMotionState = new btDefaultMotionState(transform);
	delete m_physicsBody->getRigidBody()->getMotionState();
	m_physicsBody->getRigidBody()->setMotionState(newMotionState);
	m_physicsBody->getRigidBody()->setLinearVelocity ({ 0.f, 0.f, 0.f });
	m_physicsBody->setOffset(offset);
}

void GameObject::setColliderRotation(glm::vec3 rotation)
{
	m_physicsBody->getRigidBody()->activate();
	m_physicsBody->getRigidBody()->getWorldTransform().setRotation({ rotation.x, rotation.y, rotation.z});
}

void GameObject::setColliderScale(glm::vec3 scale)
{
	m_physicsBody->getRigidBody()->activate(true);
	delete m_physicsBody->getRigidBody()->getCollisionShape();
	btVector3 newScale{ scale.x, scale.y, scale.z };
	btBoxShape* box = new btBoxShape(newScale);
	m_physicsBody->getRigidBody()->setCollisionShape(box);
}



void GameObject::PlayerCollision(GameObject* collided)
{
	if (collided->getType() == EXIT_WALL) {
		printf("LEVEL WIN !!!\n");
	}
}

void GameObject::EnemyCollision(GameObject* collided)
{
	if(collided->getType() == BULLET_TYPE)
	{
		if (collided->getState()) {
			m_transform->WorldPosition = { 0.f, -50.f, 0.f };
			setColliderPos(m_physicsBody->m_offset);
			m_physicsBody->getRigidBody()->setActivationState(ISLAND_SLEEPING);
			activeObject = false;
			collided->setState(false);
			collided->getPhysicsComponent()->getRigidBody()->setLinearVelocity({ 0.f, 0.f ,0.f });
		}
	}

	if (collided->getType() == PLAYER)
	{
		m_transform->WorldPosition = { 0.f, -50.f, 0.f };
		setColliderPos(m_physicsBody->m_offset);
		m_physicsBody->getRigidBody()->setActivationState(ISLAND_SLEEPING);
		activeObject = false;
		constexpr int damage = 60;
		*collided->health -= damage;
	}
}

void GameObject::BulletCollision(GameObject* collided)
{
	
}

