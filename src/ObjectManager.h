#pragma once
#include <string>
#include <map>
#include <vector>

class GameObject;
class Renderer;

class ObjectManager
{
public:
	ObjectManager(Renderer* rend) : p_renderer(rend) {}
	~ObjectManager();

	void addObject(std::string name, GameObject* object) { objects.insert({ name, object });}
	GameObject* getObject(std::string val) { return objects[val]; }
	std::map< std::string, GameObject*>& getObjects() { return objects; }

	void removeNullObjects();
	void removeEnemies();
	
	void UpdateAndRender() const;
	void ImGuiUpdate();

private:
	float distanceCull = 35.f;
	Renderer* p_renderer;
	std::map<std::string , GameObject*> objects;
};
