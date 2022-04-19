#include "Renderer.h"

#include "Bullet.h"
#include "Bullet.h"
#include "Mesh.h"
#include "Model.h"
#include "GameObject.h"
#include "Texture.h"

#ifdef RASPBERRY
#include "external/imgui/imgui.h"
#elif WIN32
#include "imgui/imgui.h"
#endif

void Renderer::Clear(Shader* shader) const
{
	glClearColor(bgColor.x, bgColor.y, bgColor.z, bgColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader->SetUniform1i("typeOfRend", renderMode);

}

void Renderer::Draw(GameObject* obj, Shader* shader)
{
	shader->SetUniform1i("typeOfRend", renderMode);
	const size_t test = obj->getModelComponent()->getSizeOfMeshes();

	for (unsigned int i = 0; i < test; i++)
	{
		Draw(obj->getModelComponent()->getMeshes(i), shader);
	}
}

void Renderer::Draw(Mesh* mesh, Shader* shader)
{
	drawCallCounter++;
	mesh->m_vao->Bind();
	mesh->m_ib->Bind();
	shader->Bind();

	// bind appropriate textures
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;

	for (unsigned int i = 0; i < mesh->m_textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		std::string number;
		auto type = mesh->m_textures[i].getTypePair();

		if (type.second == DIFFUSE)
			number = std::to_string(diffuseNr++);
		else if (type.second == SPECULAR)
			number = std::to_string(specularNr++);
		else if (type.second == NORMAL)
			number = std::to_string(normalNr++);

		shader->SetUniform1i((type.first + number), i);
		glBindTexture(GL_TEXTURE_2D, mesh->m_textures[i].getID());
	}

	if (!mesh->usesIndexBuffer) {
		glDrawArrays(typeOfRend, 0, mesh->vertLines);
	}
	else if ( mesh-> usesIndexBuffer)
	{
		glDrawElements(typeOfRend, mesh->m_ib->getCount(), GL_UNSIGNED_INT, nullptr);
	}

	mesh->m_vao->Unbind();
	mesh->m_ib->Unbind();
	
}

void Renderer::Draw(Model* model, Shader* shader)
{
	for (unsigned int i = 0; i < model->getSizeOfMeshes(); i++)
	{
		Draw(model->getMeshes(i), shader);
	}
}

void Renderer::ImGuiLogic()
{
	ImGui::Text("Draw Calls: %i ", drawCallCounter);
	ImGui::Checkbox("Wireframe Draw:", &wireframeMode);
	ImGui::SliderInt("Display Maps", &renderMode, 0, 5);
	ImGui::ColorEdit4("BG Color", &bgColor.x);

	if (!wireframeMode) { typeOfRend = triangles; }
	if (wireframeMode) { typeOfRend = lines; }
 }
