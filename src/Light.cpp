#include "Light.h"
#include "Shader.h"
#include "GameObject.h"

#ifdef RASPBERRY
#include "external/imgui/imgui.h"
#elif WIN32
#include "imgui/imgui.h"
#endif

DirectionalLight::DirectionalLight(Model* objRep, Camera* cam, Shader* shader, std::string name, 
	glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec)
	: direction(dir)
{
	lightRep = new GameObject(objRep, cam, shader);
	uniformName = name;
	p_shader = shader;
	ambient = amb;
	diffuse = diff;
	specular = spec;
}

void DirectionalLight::ImGuiUpdate()
{
	if(ImGui::TreeNode(uniformName.c_str()))
	{
		ImGui::SliderFloat3("Position", &direction.x, -1.f, 1.f);
		ImGui::ColorEdit3("Light Amb", &ambient.x);
		ImGui::ColorEdit3("Light Diff", &diffuse.x);
		ImGui::SliderFloat3("Light Scale", &diffuse.x, 0.f, 50.f);
		ImGui::ColorEdit3("Light Spec", &specular.x);

		ImGui::TreePop();
	}
}

void DirectionalLight::Update()
{
	p_shader->SetUniform3f(uniformName + ".direction", direction.x, direction.y, direction.z);
	p_shader->SetUniform3f(uniformName + ".ambient", ambient.r, ambient.g, ambient.b);
	p_shader->SetUniform3f(uniformName + ".diffuse", diffuse.r, diffuse.g, diffuse.b);
	p_shader->SetUniform3f(uniformName + ".specular", specular.r, specular.g, specular.b);
}



PointLight::PointLight(Model* objRep, Camera* cam, Shader* shader, std::string name, 
	glm::vec3 pos, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, glm::vec3 falloff)
	: position(pos), constant(falloff.x), linear(falloff.y), quadratic(falloff.z)
{
	lightRep = new GameObject(objRep, cam, shader, pos);
	uniformName = name;
	p_shader = shader;
	ambient = amb;
	diffuse = diff;
	specular = spec;
}

void PointLight::ImGuiUpdate()
{
	if (ImGui::TreeNode(uniformName.c_str()))
	{
		ImGui::DragFloat3("Position", &position.x);
		ImGui::ColorEdit3("Light Amb", &ambient.x);
		ImGui::ColorEdit3("Light Diff", &diffuse.x);
		ImGui::ColorEdit3("Light Spec", &specular.x);
		ImGui::SliderFloat("Light Constant", &constant, 0.f, 1.f);
		ImGui::SliderFloat("Light Linear", &linear, 0.f, 1.f);
		ImGui::SliderFloat("Light Quad", &quadratic, 0.f, 1.f);
		ImGui::TreePop();
	}
}


void PointLight::Update()
{
	
	p_shader->SetUniform3f("pos" + uniformName, position.x, position.y, position.z);
	p_shader->SetUniform3f(uniformName + ".ambient", ambient.r, ambient.g, ambient.b);
	p_shader->SetUniform3f(uniformName + ".diffuse", diffuse.r, diffuse.g, diffuse.b);
	p_shader->SetUniform3f(uniformName + ".specular", specular.r, specular.g, specular.b);

	p_shader->SetUniform1f(uniformName + ".constant", constant);
	p_shader->SetUniform1f(uniformName + ".linear", linear);
	p_shader->SetUniform1f(uniformName + ".quadratic", quadratic);
}

LightManager::LightManager(Shader* shader)
	:shader(shader) {}

LightManager::~LightManager()
{
	for (auto light : lights) { delete light; }
}

void LightManager::Update(glm::vec3 camPos)
{
	shader->Bind();
	shader->SetUniform1i("numPointLights", numPointLight);
	shader->SetUniform3f("viewPos", camPos.x, camPos.y, camPos.z);

	for (auto light : lights)
	{
		light->Update();
	}
}

void LightManager::ImGuiUpdate()
{
	if(ImGui::TreeNode("Light Settings"))
	{
		for (auto light : lights)
		{
			light->ImGuiUpdate();
		}

		ImGui::TreePop();
	}
}
