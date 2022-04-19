#pragma warning(disable : 4100) //Virtual class, which needs this value

#include "TitleScreen.h"
#ifdef RASPBERRY
#include <EGL/egl.h>
#elif WIN32
#include <GLFW/glfw3.h>
#endif


TitleScreen::TitleScreen(Renderer* renderer, Camera* cam, Shader* shader, UI* ui, btDiscreteDynamicsWorld* physicsWorld,
                         PhysicsManager* physicsmanager, bool keyPress[65536])
	:renderer(renderer), cam(cam), shader(shader), physicsWorld(physicsWorld), p_physicsManager(physicsmanager), ui(ui)
{
	keyState = keyPress;
}
void TitleScreen::Update(float dt)
{

}

void TitleScreen::Render()
{

}

void TitleScreen::UpdateImGui()
{

}

void TitleScreen::HandleEvents()
{

#ifdef RASPBERRY

	if (keyState[XK_P])
	{
		setNextState(STATE_LEVEL_1);
	}
#elif WIN32

	if (keyState[GLFW_KEY_RIGHT_BRACKET])
	{
		setNextState(STATE_LEVEL_1);
	}
#endif
	

	
}

TitleScreen::~TitleScreen()
= default;
