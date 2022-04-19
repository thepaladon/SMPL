#include "Transform.h"

#ifdef RASPBERRY
#include <glm/ext/matrix_transform.hpp>
#include "external/imgui/imgui.h"
#elif WIN32
#include <glm/ext/matrix_transform.hpp>
#include "imgui/imgui.h"
#endif

Transform::Transform()
{
	//default values in case none specified
	setPosition(glm::vec3(0.f, 0.f, 0.f)); 
	setRotation(glm::vec3(0.f, 0.f, 0.f));
	setScale(glm::vec3(1.f, 1.f, 1.f));

	//makes the trasnaltion matrix
	mTranslationMatrix = glm::translate(mTranslationMatrix, WorldPosition);

	//makes the rotation matrix
	MakeRotationMatrix();

	//makes the scale matrix
	mScaleMatrix = glm::scale(mScaleMatrix, Scale);

	Model = mTranslationMatrix * mRotationMatrix * mScaleMatrix;
}

Transform::Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	setPosition(position);
	setRotation(rotation);
	setScale(scale);

	//makes the trasnaltion matrix
	mTranslationMatrix = glm::translate(mTranslationMatrix, WorldPosition);

	//makes the rotation matrix
	MakeRotationMatrix();

	//makes the scale matrix
	mScaleMatrix = glm::scale(mScaleMatrix, Scale);

	Model = mTranslationMatrix * mRotationMatrix * mScaleMatrix;
}

Transform::~Transform()
{
}

void Transform::UpdateModelMatrix()
{
	//set the matrices we use to I	
	mTranslationMatrix = glm::mat4(1.0f);
	mRotationMatrix = glm::mat4(1.0f);
	mScaleMatrix = glm::mat4(1.0f);

	mScaleMatrix = glm::scale(mScaleMatrix, Scale);
	MakeRotationMatrix();
	mTranslationMatrix = glm::translate(mTranslationMatrix, WorldPosition);

	Model = mTranslationMatrix * mRotationMatrix * mScaleMatrix;
}

void Transform::MakeRotationMatrix()
{
	// set rotationx
	RotationMatrixX = glm::mat4(1.0f);
	RotationMatrixX[1][1] = cosf(Rotation.x);
	RotationMatrixX[2][1] = -sinf(Rotation.x);
	RotationMatrixX[1][2] = sinf(Rotation.x);
	RotationMatrixX[2][2] = cosf(Rotation.x);
	//set rotationy
	RotationMatrixY = glm::mat4(1.0f);
	RotationMatrixY[0][0] = cosf(Rotation.y);
	RotationMatrixY[2][0] = sinf(Rotation.y);
	RotationMatrixY[0][2] = -sinf(Rotation.y);
	RotationMatrixY[2][2] = cosf(Rotation.y);
	//set rotationz
	RotationMatrixZ = glm::mat4(1.0f);
	RotationMatrixZ[0][0] = cosf(Rotation.z);
	RotationMatrixZ[1][0] = -sinf(Rotation.z);
	RotationMatrixZ[0][1] = sinf(Rotation.z);
	RotationMatrixZ[1][1] = cosf(Rotation.z);

	// now the combined rotation	
	mRotationMatrix = RotationMatrixX * RotationMatrixY * RotationMatrixZ;
}

void Transform::setPosition(glm::vec3 positions)
{
	WorldPosition = positions;
}

void Transform::setRotation(glm::vec3 rotations)
{
	Rotation = glm::radians(rotations);
}

void Transform::setScale(glm::vec3 scale)
{
	Scale = scale;
}
