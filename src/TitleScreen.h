#pragma once
#include "GameState.h"

class TitleScreen : public GameState
{
public:
	TitleScreen(Renderer* renderer, Camera* cam, Shader* shader, UI* ui, btDiscreteDynamicsWorld* physicsWorld, PhysicsManager* physicsmanager, bool keyState[65536]);

	void Update(float dt) override;
	void Render() override;
	void UpdateImGui() override;
	void HandleEvents() override;
	~TitleScreen() override;


private:

	//References
	bool* keyState; //keys
	UI* ui;
	btDiscreteDynamicsWorld* physicsWorld;
	PhysicsManager* p_physicsManager;
	Renderer* renderer;
	Camera* cam;
	Shader* shader;
};
