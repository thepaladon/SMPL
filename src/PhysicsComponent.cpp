#include <algorithm>
#include <iterator>

#include "PhysicsComponent.h"
#include <glm/gtx/quaternion.hpp>
#include "GameObject.h"

btBoxShape* createBoxShape(const btVector3& halfExtents)
	{
		btBoxShape* box = new btBoxShape(halfExtents);
		return box;
	}

	btRigidBody* createRigidBodyFull(float mass, glm::vec3 origin, glm::vec3 size, btDynamicsWorld* world, short aGroup, short aMask)
	{
		btCollisionShape* shape = new btBoxShape({ size.x, size.y, size.z });

		btAssert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE));
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			shape->calculateLocalInertia(mass, localInertia);

		btTransform transform;
		transform.setIdentity();
		transform.setOrigin({ origin.x, origin.y, origin.z });

		btDefaultMotionState* myMotionState = new btDefaultMotionState(transform);
		btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, shape, localInertia);
		btRigidBody* body = new btRigidBody(cInfo);

		body->setUserIndex(-1);
		world->addRigidBody(body, aGroup, aMask);
		return body;
	}

	btRigidBody* Initialize(btCollisionShape* Shape, float mass, glm::vec3 transform, btDynamicsWorld* world)
	{
		btTransform btTransform;
		btTransform.setIdentity();
		btTransform.setOrigin(btVector3(transform.x, transform.y, transform.z));

		// Defines our position in the physics
		btMotionState* state = new btDefaultMotionState(btTransform);

		// mass of 0 = static
		// static objects have an interia of 0
		// mass > 0 = dynamic shape
		// if dynamic, calculated inertia with collision shape
		btVector3 inertia{ 0.0f, 0.0f, 0.0f };
		if (mass > 0)
		{
			Shape->calculateLocalInertia(mass, inertia);
		}

		btRigidBody* rigidbody = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(mass, state, Shape, inertia));
		world->addRigidBody(rigidbody);

		return rigidbody;
	}

btRigidBody* createRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape, btDynamicsWorld* world, short aGroup, short aMask)
	{
		btAssert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE));
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			shape->calculateLocalInertia(mass, localInertia);

		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, shape, localInertia);
		btRigidBody* body = new btRigidBody(cInfo);

		//body->setUserIndex(-1);
		//body->forceActivationState(true);
		//body->activate(true);
		world->addRigidBody(body, aGroup, aMask);
		
		return body;

	}

	PhysicsComponent::PhysicsComponent(float mass, glm::vec3 position, glm::vec3 p_offset, glm::vec3 p_scale, btDynamicsWorld* world, short aGroup, short aMask)
	{
		btCollisionShape* box = new btBoxShape(btVector3(p_scale.x * 0.5f, p_scale.y * 0.5f, p_scale.z * 0.5f));
		m_scale = p_scale;
		m_offset = p_offset;
		btTransform transform;
		transform.setIdentity();
		transform.setOrigin(btVector3(position.x, position.y, position.z));

		m_rigidBody = createRigidBody(mass, transform, box, world, aGroup, aMask);
	}

	//NOT working convex shape constructor
	PhysicsComponent::PhysicsComponent(float mass, glm::vec3 position, glm::vec3 p_offset, btDynamicsWorld* world, Model* model )
	{
		//old convex shape code
		/*std::vector<btScalar> test;
		btTriangleMesh* triangleMeshTerrain = new btTriangleMesh(); // make a mesh for the terrain
		btBvhTriangleMeshShape* shape;
		for (int i = 0; i < model->getSizeOfMeshes(); i++) {
			for (auto vertex : model->getMeshes(i)->m_vertices) {
				btVector3 pos;
				test.push_back(vertex.Position.x);
				test.push_back(vertex.Position.y);
				test.push_back(vertex.Position.z);
			}
		}

		//shape->optimizeConvexHull();
		shape->initializePolyhedralFeatures();

		const btVector3 points[5] =
		{
			btVector3(0.0f,1.0f,0.0f),
			btVector3(-0.5f, 0.0f, -0.5f),
			btVector3(-0.5f, 0.0f, 0.5f),
			btVector3(0.5f, 0.0f, -0.5f),
			btVector3(0.5f, 0.0f, 0.5f)
		};

		btConvexHullShape* convexShape = new btConvexHullShape(&points[0].x(), 5);
		//convexShape->initializePolyhedralFeatures();
		convexShape->setLocalScaling({ 1.f, 1.f, 1.f });
		m_scale = glm::vec3(1.f);
		m_offset = p_offset;
		btTransform transform;
		transform.setIdentity();
		transform.setRotation(btQuaternion(0.f, 0.f, 0.f));
		transform.setOrigin(btVector3(position.x, position.y, position.z));
		
		m_physicsBody = createRigidBody(mass, transform, convexShape, world, );
		//m_physicsBody = Initialize(convexShape, mass, position, world);
		//m_physicsBody->setActivationState(DISABLE_DEACTIVATION);*/
	}

	PhysicsComponent::~PhysicsComponent()
	{
	}

	glm::mat4 PhysicsComponent::getPhysicalModelMatrix()
	{
		modelMatrix = glm::mat4(1.f);
		
		glm::mat4 transformMat, rotationMat;

		transformMat = glm::translate(transformMat, { m_rigidBody->getWorldTransform().getOrigin().x() + m_offset.x,m_rigidBody->getWorldTransform().getOrigin().y() + m_offset.y, m_rigidBody->getWorldTransform().getOrigin().z() + m_offset.z });

		glm::quat quatRot(
			m_rigidBody->getWorldTransform().getRotation().w(),
			m_rigidBody->getWorldTransform().getRotation().x(),
			m_rigidBody->getWorldTransform().getRotation().y(),
			m_rigidBody->getWorldTransform().getRotation().z()
		);
		
		rotationMat = glm::toMat4(quatRot);

		modelMatrix = transformMat;
		modelMatrix = modelMatrix * rotationMat;
		modelMatrix = glm::scale(modelMatrix, m_scale);

		return modelMatrix;
	}

void PhysicsComponent::LimitVelocity(float maxVelocity)
{
	btVector3 velocity = m_rigidBody->getLinearVelocity();

	if (velocity.length2() > maxVelocity * maxVelocity)
	{
		velocity.normalize();
		velocity *= maxVelocity;
		m_rigidBody->setLinearVelocity(velocity);
	}
}


PhysicsManager::PhysicsManager(Camera* p_cam)
	: p_cam(p_cam), ref_objects(nullptr)
{
	std::string rootFilepath = "ERROR";
#ifdef RASPBERRY
		rootFilepath = "../../../";
#elif WIN32
	rootFilepath = "../";
#endif

	m_shader = new Shader(rootFilepath + "res/shaders/physicsDebug");
	SetupMesh();
}

	PhysicsManager::~PhysicsManager()
	{
	}

	void PhysicsManager::SetupMesh()
	{
		m_mesh = new Mesh("debug", TheLines.data(), static_cast<unsigned int>(TheLines.size()) * sizeof(TheLines[0]), nullptr, 0);
	}

	void PhysicsManager::UpdateMesh()
	{
		m_mesh->UpdateMesh(TheLines.data(), static_cast<unsigned int>(TheLines.size()) * sizeof(TheLines[0]));
	}

	void PhysicsManager::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
	{
		LineValues Line;
		btVector3 scolor = btVector3(1, 0, 1);

		Line.p1.x = from.x();
		Line.p1.y = from.y();
		Line.p1.z = from.z();

		Line.Color1.x = color.x();
		Line.Color1.y = color.y();
		Line.Color1.z = color.z();

		Line.p2.x = to.x();
		Line.p2.y = to.y();
		Line.p2.z = to.z();

		Line.Color2.x = color.x();
		Line.Color2.y = color.y();
		Line.Color2.z = color.z();

		m_mesh->m_vao->Bind();

		TheLines.push_back(Line);
	}

	void PhysicsManager::DrawFinal(glm::mat4 Projection, glm::mat4 View)
	{
		m_mesh->UpdateMesh(TheLines.data(), static_cast<unsigned>(TheLines.size()) * sizeof(TheLines[0]));
		m_mesh->m_vao->Bind();
		m_shader->Bind();
		glm::mat4 MVP = Projection * View * glm::mat4(1.0);
		m_shader->SetUniformMat4f("MVP", MVP);

		glDrawArrays(GL_LINES, 0, static_cast<unsigned>(TheLines.size() * 2)); // there are 2 componants to send

		m_mesh->m_vao->Unbind();
		m_shader->Unbind();
		TheLines.clear();
	}

void PhysicsManager::CheckForCollision(btDispatcher* m_pDispatcher)
{
	CollisionPairs pairsThisUpdate;

	for (int i = 0; i < m_pDispatcher->getNumManifolds(); ++i) {

		btPersistentManifold* pManifold = m_pDispatcher->getManifoldByIndexInternal(i);

		if (pManifold->getNumContacts() > 0) {
			const btRigidBody* pBody0 = static_cast<const btRigidBody*>(pManifold->getBody0());
			const btRigidBody* pBody1 = static_cast<const btRigidBody*>(pManifold->getBody1());

			bool const swapped = pBody0 > pBody1;
			const btRigidBody* pSortedBodyA = swapped ? pBody1 : pBody0;
			const btRigidBody* pSortedBodyB = swapped ? pBody0 : pBody1;

			CollisionPair thisPair = std::make_pair(pSortedBodyA, pSortedBodyB);

			pairsThisUpdate.insert(thisPair);

			if (m_pairsLastUpdate.find(thisPair) == m_pairsLastUpdate.end()) {
				CollisionEvent((btRigidBody*)pBody0, (btRigidBody*)pBody1);
			}
		}
	}

	CollisionPairs removedPairs;
	std::set_difference(m_pairsLastUpdate.begin(), m_pairsLastUpdate.end(),
		pairsThisUpdate.begin(), pairsThisUpdate.end(),
		std::inserter(removedPairs, removedPairs.begin()));

	for (CollisionPairs::const_iterator iter = removedPairs.begin(); iter != removedPairs.end(); ++iter) {
		SeparationEvent((btRigidBody*)iter->first, (btRigidBody*)iter->second);
	}

	m_pairsLastUpdate = pairsThisUpdate;
}

void PhysicsManager::CollisionEvent(btRigidBody* pBody0, btRigidBody* pBody1) {

	GameObject* obj0 = nullptr;
	GameObject* obj1 = nullptr;

	if (ref_objects) {
		for (auto object : *ref_objects) {
			if (object.second->getPhysicsComponent()->getRigidBody() == pBody0)
			{
				obj0 = object.second;
			}
			else if (object.second->getPhysicsComponent()->getRigidBody() == pBody1)
			{
				obj1 = object.second;
			}
		}
	}

	if (!obj1 || !obj0)  return; 
	//If they don't exist in the ObjectManager they're unimportant or map.

	switch (obj0->getType())
	{
	case NONE: break;
	case GROUND: break;
	case PLAYER:
		obj0->PlayerCollision(obj1);
		break;
	case ENEMY:
		obj0->EnemyCollision(obj1);
		break;
	case BULLET_TYPE:
		obj0->BulletCollision(obj1);
		break;
	case ALL: break;
	default:;

	}

	switch (obj1->getType())
		{
		case NONE: break;
		case GROUND: break;
		case PLAYER:
			obj1->PlayerCollision(obj0);
			break;
		case ENEMY:
			obj1->EnemyCollision(obj0);
			break;
		case BULLET_TYPE:
			obj1->BulletCollision(obj0);
			break;
		case ALL: break;
		default:;
		}
		
	
}

void PhysicsManager::SeparationEvent(btRigidBody* pBody0, btRigidBody* pBody1) {

	if (ref_objects) {
		for (auto object : *ref_objects) {
			if (object.second->getPhysicsComponent()->getRigidBody() == pBody0 || object.second->getPhysicsComponent()->getRigidBody() == pBody1)
			{
				object.second->OnCollisionExitExecute();
				return;
			}
		}
	}
}