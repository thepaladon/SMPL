#include "Camera.h"

#include <iostream>
#include <glm/ext/matrix_clip_space.hpp>

#include "GameObject.h"

#ifdef RASPBERRY
#include "external/imgui/imgui.h"
#elif WIN32
#include "imgui/imgui.h"
#endif

Camera::Camera(int p_win_width, int p_win_height)
{
	cameraPos = glm::vec3(0.f, 0.f, 0.f);
	win_width = p_win_width; win_height = p_win_height;

#ifdef RASPBERRY
	NearDistanceCull = 0.1f;
	FarDistanceCull = 200.f;
#elif WIN32
	NearDistanceCull = 1.5f;
	FarDistanceCull = 1000000000;
#endif

	projection = glm::perspective(glm::radians(fov), (float)win_width / (float)win_height, NearDistanceCull, FarDistanceCull);
}

void Camera::Update(int p_win_width, int p_win_height, float deltaTime)
{
	win_width = p_win_width; win_height = p_win_height;

	cameraSpeed = static_cast<float>(speedMultiplier) * deltaTime;
	mouseSensitivity = static_cast<float>(sensitivityMultiplier) * deltaTime;
	glm::vec3 lookAt;
	if (freeCam) {
		pitch = glm::clamp(pitch, -89.f, 89.f);
		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraDirection = glm::normalize(direction);
		cameraRight = glm::normalize(glm::cross(up, cameraDirection));
		lookAt = cameraPos + cameraDirection;
	}
	else
	{
		if (lookAtObject) {
			if (lookAtObject->getTransComponent()) {
				glm::vec3 targetPos = lookAtObject->getTransComponent()->WorldPosition;
				cameraDirection = targetPos;
				cameraRight = glm::normalize(glm::cross(up, cameraDirection));
				cameraPos = camCircleLogic(targetPos, offsetObj.y, offsetObj.x, rotationFromObj);
			}
		}
		lookAt = cameraDirection;
	}

	view = glm::lookAt(cameraPos, lookAt, up);
	projection = glm::perspective(glm::radians(fov), (float)win_width / (float)win_height, NearDistanceCull, FarDistanceCull);
}

void Camera::UpdateImGui()
{
	if(ImGui::TreeNode("Camera Settings"))
	{
		ImGui::Text("Screen Resolution: %ix%i", win_width, win_height);
		ImGui::DragFloat3("Camera Position", &cameraPos.x, .5f);
		ImGui::SliderFloat2("Pitch and Yaw", &pitch, -360.f, 360.f);
		ImGui::DragFloat("FOV", &fov, .5f, 1.f);
		ImGui::DragInt("Mouse Sensitivity", &sensitivityMultiplier,1.f, 1);
		ImGui::DragInt("Camera Speed", &speedMultiplier, 1.f, 1);
		ImGui::DragFloat("Near Distance Cull", &NearDistanceCull, 1.f, 1.f);
		ImGui::DragFloat("Far Distance Cull", &FarDistanceCull, 1.f, 1.f);

		ImGui::DragFloat2("Bound Cam Offset", &offsetObj.x, 1.f);
		ImGui::DragFloat("Bound Cam Rotation", &rotationFromObj, 1.f);

		ImGui::TreePop();
	}
}

void Camera::moveForward(const Direction dir)
{
	cameraPos += cameraSpeed * cameraDirection * static_cast<float>(dir);
}

void Camera::moveSideways(const Direction dir)
{
	cameraPos += cameraRight * cameraSpeed * static_cast<float>(dir);
}

void Camera::moveVertical(const Direction dir)
{
	cameraPos += cameraSpeed * up * static_cast<float>(dir);
}

void Camera::moveLookAT(const bool axis, const Direction dir)
{
	if(axis == 0)
	{
		pitch += mouseSensitivity * static_cast<float>(dir);

	}else if(axis == 1)
	{
		yaw += mouseSensitivity * static_cast<float>(-dir);
	}
}

void Camera::rotateRelativeToObj(Direction dir)
{
	rotationFromObj += static_cast<float>(dir) * cameraSpeed * 5;
}

void Camera::moveRelativeToObj(Direction dir)
{
	offsetObj.x -= static_cast<float>(dir) * cameraSpeed * 5;
}

void Camera::moveRelativeToObjVertical(Direction dir)
{
	offsetObj.y += static_cast<float>(dir) * cameraSpeed * 5;
}

void Camera::moveLookAt(int x, int y)
{
	pitch += mouseSensitivity * static_cast<float>(x);
	yaw += mouseSensitivity * static_cast<float>(y);
}


glm::vec3 Camera::camCircleLogic(const glm::vec3 pos, const float y_offset, const float r, const float degree)
{
	const float r1 = static_cast<float>(degree) * 3.14159265358f / 360.f;
	return {(pos.x - r * sinf(r1)), pos.y + y_offset, (pos.z - r * cosf(r1))};
}