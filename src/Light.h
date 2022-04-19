#pragma once
#include <string>
#include <vector>
#include <glm/vec3.hpp>

class Camera;
class GameObject;
class Model;
class Shader;



class Light
{
public:
	virtual ~Light() = default;

	virtual void Update() = 0;
	virtual void ImGuiUpdate() = 0;

	Shader* p_shader = nullptr;
	GameObject* lightRep = nullptr;

	std::string uniformName = "ERROR, LIGHT NOT SPECIFIED";
	//I could add those into a struct - Dragos fb
	glm::vec3 ambient{ glm::vec3(1.f, 1.f, 1.f) };
	glm::vec3 diffuse{ glm::vec3(1.f, 1.f, 1.f) };
	glm::vec3 specular{ glm::vec3(1.f, 1.f, 1.f) };
};


class DirectionalLight : public Light
{
public:
	DirectionalLight(Model* objRep, Camera* cam, Shader* shader, std::string name,
		glm::vec3 dir = glm::vec3(-0.2f, -1.0f, -0.3f),
		glm::vec3 amb = glm::vec3(1.f), glm::vec3 
		diff = glm::vec3(1.f), 
		glm::vec3 spec = glm::vec3(1.f));

	void Update() override;
	void ImGuiUpdate() override;

private:
	glm::vec3 direction = glm::vec3(-0.2f, -1.0f, -0.3f);
};

class PointLight : public Light
{
public:
	PointLight(Model* objRep, Camera* cam, Shader* shader, std::string name,
		glm::vec3 pos = glm::vec3(0.f), 
		glm::vec3 amb = glm::vec3(1.f), 
		glm::vec3 diff = glm::vec3(1.f), 
		glm::vec3 spec = glm::vec3(1.f),
		glm::vec3 falloff = glm::vec3(0.f, 0.03f, 0.05f));

	void Update() override;
	void ImGuiUpdate() override;
	void setPos(glm::vec3 newPos) { position = newPos; }

private:
	glm::vec3 position{ glm::vec3(1.2f, 1.0f, 2.0f) };
	float constant = 1.f;
	float linear = 0.09f;
	float quadratic = 0.032f;
};

class LightManager
{
public:
	LightManager(Shader* shader);
	~LightManager();

	void addPointLight(Light* light) { lights.push_back(light); numPointLight++; }
	void addDirectionalLight(Light* light) { lights.push_back(light);}

	PointLight* getLight(int i) { return static_cast<PointLight*>(lights[i]); }

	void deleteLights() { lights.clear(); }

	void Update(glm::vec3 camPos);
	void ImGuiUpdate();

private:
	int numPointLight = 0;
	Shader* shader;
	std::vector<Light*> lights; 

};