#pragma once
#include <glm/glm.hpp>

class Transform
{
public:
	Transform();
	Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	~Transform();

	void UpdateModelMatrix();

	// set up the rotation matrices
	void MakeRotationMatrix();

	void setPosition(glm::vec3 positions);
	void setRotation(glm::vec3 rotations);
	void setScale(glm::vec3 scale);

	bool rebuildModel = false;
	glm::mat4  Model = glm::mat4(1.f);

	glm::vec3 WorldPosition;   // where am I in the world?
	glm::vec3 Rotation;
	glm::vec3 Scale = glm::vec3(1.f);

	glm::mat4 mTranslationMatrix = glm::mat4(1.0f);
	glm::mat4 mRotationMatrix = glm::mat4(1.0f);
	glm::mat4 mScaleMatrix = glm::mat4(1.0f);

	glm::mat4 RotationMatrixX;
	glm::mat4 RotationMatrixY;
	glm::mat4 RotationMatrixZ;
};
