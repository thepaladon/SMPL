#pragma once

#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/ext/matrix_transform.hpp>

class GameObject;

enum Direction
{
	Forward = 1,
	Left = 1,
	Backward = -1,
	Right = -1
};

class Camera
{
public:
	Camera(int p_win_width, int p_win_height);
	~Camera() = default;;

	void Update(int p_win_width, int p_win_height, float deltaTime);
	void UpdateImGui();
	glm::mat4 getViewMatrix() const { return view; };
	glm::mat4 getProjectionMatrix() const { return projection; };

	void moveForward(Direction dir);
	void moveSideways(Direction dir);
	void moveVertical(Direction dir);
	void moveLookAT(bool axis, Direction dir);

	void rotateRelativeToObj(Direction dir);
	void moveRelativeToObj(Direction dir);
	void moveRelativeToObjVertical(Direction dir);

	void moveLookAt(int x, int y);

	void setFreeCameraMode(bool newMode) { freeCam = newMode; }
	void setLookAtObject(GameObject* obj) { lookAtObject = obj; }

	glm::vec3 camCircleLogic(const glm::vec3 pos, const float y_offset, const float r, const float degree);

	inline glm::vec3 getCamPos() const { return cameraPos; }
	inline glm::vec3 getCamLookAt() const { return cameraDirection - cameraPos; }
	inline glm::vec3 getCamRight() const { return cameraRight; }
	inline float getCamFOV() const { return fov; }
	inline float getPitch() const { return pitch; }
	inline float getYaw() const { return yaw; }
	inline bool isFreeCam() const { return freeCam; }

private:
	bool freeCam = false; // if not free, then it's bound to some object
	GameObject* lookAtObject;
	glm::vec2 offsetObj{34.f, 25.f};
	float rotationFromObj = 0;

	glm::vec3 cameraPos;
	glm::mat4 view;
	glm::mat4 projection;
	int win_width, win_height;
	float FarDistanceCull = 0.f;
	float NearDistanceCull = 0.f;

	float fov = 60.f;
	float cameraSpeed;
	float mouseSensitivity;

	int sensitivityMultiplier = 70;
	int speedMultiplier = 20;

	float pitch = 0.f;
	float yaw = 0.f;

	glm::vec3 cameraDirection;
	glm::vec3 cameraRight;
	const glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	
};


