#pragma once
#pragma warning (disable : 26495)
#pragma warning (disable : 4100)

#include <set>

#include "Mesh.h"
#include "Camera.h"
#include "ObjectManager.h"
#include"bullet/btBulletDynamicsCommon.h"


enum ObjectTypes
{
	NONE = 0,
	GROUND = 1 << 0,
	PLAYER = 1 << 1,
	ENEMY = 1 << 2,
	BULLET_TYPE = 1 << 3,
	EXIT_WALL = 1 << 4,
	ALL = 0b11111
};

class Model;
	typedef std::pair<const btRigidBody*, const btRigidBody*> CollisionPair;
	typedef std::set<CollisionPair> CollisionPairs;
	void CollisionEvent(btRigidBody* pBody0, btRigidBody* pBody1);
	void SeparationEvent(btRigidBody* pBody0, btRigidBody* pBody1);

	btBoxShape* createBoxShape(const btVector3& halfExtents);	
	btRigidBody* createRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape, btDynamicsWorld* world, short aGroup, short aMask);
	btRigidBody* createRigidBodyFull(float mass, glm::vec3 origin, glm::vec3 size, btDynamicsWorld* world, short aGroup, short aMask);
	//Stefan's Initialize
	btRigidBody* Initialize(btCollisionShape* Shape, float mass, glm::vec3 transform, btDynamicsWorld* world);

struct LineValues
	{
		glm::vec3 p1;
		glm::vec3 Color1;
		glm::vec3 p2;
		glm::vec3 Color2;
	};

	class PhysicsComponent
	{
	public:
		PhysicsComponent(float mass, glm::vec3 position, glm::vec3 p_offset, glm::vec3 p_scale, btDynamicsWorld* world, short aGroup, short aMask);
		PhysicsComponent(float mass, glm::vec3 position, glm::vec3 p_offset, btDynamicsWorld* world, Model* model);
		~PhysicsComponent();

		btRigidBody* getRigidBody() { return m_rigidBody; }
		glm::mat4 getPhysicalModelMatrix();
		glm::vec3 getOffset() { return m_offset; }

		void setOffset(glm::vec3 offset) { m_offset = offset; }
		void setScale (glm::vec3 scale) { m_scale = scale; }

		void LimitVelocity(float maxVelocity);

		glm::vec3 m_scale;
		glm::vec3 m_offset;
	private:
		btRigidBody* m_rigidBody;
		glm::mat4 modelMatrix;
	};

	class PhysicsManager : public btIDebugDraw
	{
	public:
		PhysicsManager(Camera* p_cam);
		~PhysicsManager();

		void passObjects(std::map <std::string, GameObject*>* p_objMan) { ref_objects = p_objMan; }
		void removeObjects() { ref_objects = nullptr; }

		void SetupMesh();
		void UpdateMesh();
		void ClearMesh() { TheLines.clear(); }

		void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override; 
		void setDefaultColors(const DefaultColors&) override {};
		
		void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override {};
		void reportErrorWarning(const char* warningString) override {};

		void draw3dText(const btVector3& location, const char* textString) override {};
		void setDebugMode(int p_debugMode) override { debugMode = p_debugMode; };
		int getDebugMode() const override { return debugMode;  };
		DefaultColors getDefaultColors() const override { return DefaultColors(); };

		void DrawFinal(glm::mat4 Projection, glm::mat4 View);

		//Collision
		void CheckForCollision(btDispatcher* m_pDispatcher);
		void CollisionEvent(btRigidBody* pBody0, btRigidBody* pBody1);
		void SeparationEvent(btRigidBody* pBody0, btRigidBody* pBody1);

		bool RenderDebug = false;
		bool UpdateDebug = true;
	protected:
		int debugMode;
		Mesh* m_mesh;
		Shader* m_shader;
		Camera* p_cam;
		std::vector<LineValues> TheLines;
		std::map<std::string, GameObject*>* ref_objects;
		CollisionPairs m_pairsLastUpdate;
	};
