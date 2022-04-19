#include "ObjectManager.h"
#include "GameObject.h"
#include "Renderer.h"

#ifdef RASPBERRY
#include "external/imgui/imgui.h"
#elif WIN32
#include "imgui/imgui.h"
#endif

ObjectManager::~ObjectManager()
{
	for (auto object : objects) {
		delete object.second;
	}
}

void ObjectManager::removeNullObjects()
{
	for (auto object : objects) {
		if (object.second == nullptr)
		{
			objects.erase(object.first);
		}
	}
}

void ObjectManager::removeEnemies()
{
	std::vector<const std::string*> toDelete;
	for (auto& object : objects)
	{
		if (object.first.find("ENEMY") != std::string::npos)
		{
			toDelete.push_back(&object.first);
		}
	}

	for (auto& key : toDelete)
	{
		objects.erase(*key);
	}
}

void ObjectManager::UpdateAndRender() const
{
	for (auto object : objects) 
	{
		if (object.second->getState()) {
			float dist = glm::distance(object.second->getTransComponent()->WorldPosition, objects.at("Player0")->getTransComponent()->WorldPosition);

			object.second->Update();
			if (dist < distanceCull) {
				p_renderer->Draw(object.second, object.second->getShader());
			}
		}
	}
}

void ObjectManager::ImGuiUpdate()
{

	if (ImGui::TreeNode("Object Settings")) {
		ImGui::SliderFloat("Near Distance Cull", &distanceCull, 0.f, 100.f);
		for (auto object : objects) {
			if (ImGui::TreeNode(object.first.c_str())) {

				ImGui::Checkbox("Active", object.second->getStateP());

				if (object.second->getPhysicsComponent() != nullptr) {
				if(ImGui::DragFloat3("Position", &object.second->getTransComponent()->WorldPosition.x, 0.25))
				{ object.second->setColliderPos(object.second->getPhysicsComponent()->m_offset); }

				if(ImGui::DragFloat3("Rotation", &object.second->getTransComponent()->Rotation.x, 0.25))
				{ object.second->setColliderRotation(object.second->getTransComponent()->Rotation);}

				if(ImGui::DragFloat3("Scale", &object.second->getTransComponent()->Scale.x, 0.25))
				{ object.second->setColliderScale(object.second->getPhysicsComponent()->m_scale * 0.5f); }

					if (ImGui::DragFloat3("Collider Pos", &object.second->getPhysicsComponent()->m_offset.x, 0.25))
					{ object.second->setColliderPos(object.second->getPhysicsComponent()->m_offset); }

					if (ImGui::DragFloat3("Collider Scale", &object.second->getPhysicsComponent()->m_scale.x, 0.25))
					{ object.second->setColliderScale(object.second->getPhysicsComponent()->m_scale * 0.5f); }

					if (ImGui::Button("Jump"))
					{
						object.second->getPhysicsComponent()->getRigidBody()->activate(true);
						btVector3 velocity3(btVector3(0.f, 25.f, 0.f) - object.second->getPhysicsComponent()->getRigidBody()->getWorldTransform().getOrigin());
						object.second->getPhysicsComponent()->getRigidBody()->applyCentralImpulse(velocity3);
						object.second->getPhysicsComponent()->getRigidBody()->applyTorque(velocity3);
					}
				}else
				{
					ImGui::DragFloat3("Position", &object.second->getTransComponent()->WorldPosition.x, 0.25);
					ImGui::DragFloat3("Rotation", &object.second->getTransComponent()->Rotation.x, 0.25);
					ImGui::DragFloat3("Scale", &object.second->getTransComponent()->Scale.x, 0.25);
				}

				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
}