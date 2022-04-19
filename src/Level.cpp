#include "Level.h"
#include "Bullet.h"
#include "Counter.h"
#include "Enemy.h"
#include "Light.h"
#include "Map.h"
#include "GameObject.h"
#include "Player.h"
#include "Renderer.h"
#include "UI.h"


#ifdef RASPBERRY
#include "external/imgui/imgui.h"
#include <EGL/egl.h>
#elif WIN32
#include "imgui/imgui.h"
#include <GLFW/glfw3.h>
#endif

#ifdef RASPBERRY
const std::string rootFilepath = "../../../";
#elif WIN32
const std::string rootFilepath("../");
#endif


Level::Level(Renderer* renderer, Camera* cam, Shader* shader, UI* ui, btDiscreteDynamicsWorld* physicsWorld, PhysicsManager* physicsmanager, bool keyPress[65536])
:renderer(renderer), cam(cam), shader(shader), physicsWorld(physicsWorld), p_physicsManager(physicsmanager), ui(ui)
{
	keyState = keyPress;
	p_physicsManager->UpdateDebug = true;
	LoadLevel();
}

Level::~Level()
{
	cam->setFreeCameraMode(true);
	p_physicsManager->removeObjects();
	for (auto player : players) { delete player; }
	for (auto gun : guns) { delete gun; }

	delete lights;
	delete enemies;
	delete map;
	delete objectManager;

	for (auto i : Counter::GetCounter().indexID)			{ glDeleteBuffers(1, &i); }
	for (auto i : Counter::GetCounter().vertexArrayID)	{ glDeleteBuffers(1, &i); }
	for (auto i : Counter::GetCounter().vertexBufferID) { glDeleteBuffers(1, &i); }
	for (auto i : Counter::GetCounter().textureIDs)		{ glDeleteTextures(1, &i); }
	//for (auto i : Counter::GetCounter().shaderIDs)		{ glDeleteProgram(i); }

	Counter::GetCounter().clearAll();
}

void Level::LoadLevel()
{
	//MUSTs
	time = 0.f;
	paused = false;
	objectManager = new ObjectManager(renderer);
	lights = new LightManager(shader);
	p_physicsManager->passObjects(&objectManager->getObjects());
	cam->setFreeCameraMode(false);
	lights->addPointLight(new PointLight(
		nullptr, cam, shader, 
		"pointLights[0]", 
		glm::vec3(0.f, 1.f, 0.f), 
		glm::vec3(248, 114, 73) / 255.f, glm::vec3(52.f, 0.f, 255.f) / 255.f, glm::vec3(1.f, 1.f, 0.f), 
	glm::vec3(0.f, 0.066f, 0.018f)));

	//Creating Player
	players.push_back(new Player(cam));
	players[0]->setPlayerObj(new GameObject(rootFilepath + "res/models/Metal_Abstract/cube.obj", cam, shader), physicsWorld);
	objectManager->addObject("Player0", players[0]->getObject());

	ui->setPlayerValues(players[0]->getHealthP(), players[0]->getMaxHealthP());

	cam->setLookAtObject(players[0]->getObject());
	guns.push_back(new GunBulletManager(cam, shader, objectManager, players[0]->getObject(), physicsWorld, ui->playerSelectedCharacter));	//Creating the player's gun

	//Creating the Map + Enemies
	enemies = new EnemyManager(players[0], cam, shader, objectManager, physicsWorld);
	map = new Map(renderer, cam, shader, physicsWorld, enemies, objectManager); //should be initialized after the player
	map->setPlayer(players[0]);

	if (firstBuild == true) {
		map->BuildLevel(rootFilepath + "res/lvl1.csv", 4.f,
			rootFilepath + "res/models/ground_rubble/cube.obj",
			rootFilepath + "res/models/wall_brick/cube.obj");
		firstBuild = false;
	}else
	{
		map->ReBuildLevel(rootFilepath + "res/lvl1.csv", 4.1f,
			rootFilepath + "res/models/ground_rubble/cube.obj",
			rootFilepath + "res/models/wall_brick/cube.obj");
	}

}

void Level::Update(float dt)
{
	time += dt;
	if(players[0]->getHealth() <= 0)
	{
		ui->setUIMode(UIMode::GAME_OVER);
		p_physicsManager->UpdateDebug = false;
	}

	if(ui->restartLevel)
	{
		RestartLevel();
		ui->restartLevel = false;
	}

	if(players[0]->getObject()->getTransComponent()->WorldPosition.y <= -5 )
	{
		ui->setUIMode(UIMode::WIN_SCREEN);
		p_physicsManager->UpdateDebug = false;
	}

	Input();
	guns[0]->Update(dt);
	players[0]->PlayerLogicUpdate(dt);

	lights->getLight(0)->setPos(players[0]->getObject()->getTransComponent()->WorldPosition);
	enemies->Update();
}

void Level::UpdateImGui()
{
	ImGui::SliderFloat("Map Cull", &map->distanceCull, 0, 500);
	lights->ImGuiUpdate();
	objectManager->ImGuiUpdate();
}

void Level::Render()
{
	lights->Update(cam->getCamPos());
	map->Draw();
	objectManager->UpdateAndRender();
}

void Level::HandleEvents()
{
	

#ifdef RASPBERRY
	//If the user has Q exit out the window
	if (keyState[XK_O])
	{
		//Quit the program
		printf("exit from level 1 \n");
		setNextState(STATE_TITLE);
	}
	else if (keyState[XK_P])
	{
		printf("Bracket test \n");

	}

#elif WIN32
	//If the user has Q exit out the window
	if (keyState[GLFW_KEY_LEFT_BRACKET])
	{
		//Quit the program
		printf("exit from level 1 \n");
		setNextState(STATE_TITLE);
	}
	else if (keyState[GLFW_KEY_RIGHT_BRACKET])
	{
		printf("Bracket test \n");

	}
#endif
}

void Level::Input()
{
#ifdef RASPBERRY
	if (!cam->isFreeCam()) {
		if (keyState[XK_w]) {
			players[0]->moveForward(Forward);
		}
		else if (keyState[XK_s])
		{
			players[0]->moveForward(Backward);
		}

		if (keyState[XK_a]) {
			players[0]->moveSideways(Left);
		}
		else if (keyState[XK_d])
		{
			players[0]->moveSideways(Right);
		}

#ifdef _DEBUG
		if (keyState[XK_space])
		{
			players[0]->jump(0.5f);
		}
#endif

		if (keyState[XK_m])
		{
			map->ReBuildLevel(rootFilepath + "res/lvl1.csv", 4.1f,
				rootFilepath + "res/models/ground_rubble/cube.obj",
				rootFilepath + "res/models/wall_brick/cube.obj");
		}

		if (keyState[XK_Right] && keyState[XK_Up])
			guns[0]->FireBullet({ -1, 1 });
		else if (keyState[XK_Left] && keyState[XK_Up])
			guns[0]->FireBullet({ 1, 1 });
		else if (keyState[XK_Left] && keyState[XK_Down])
			guns[0]->FireBullet({ 1, -1 });
		else if (keyState[XK_Right] && keyState[XK_Down])
			guns[0]->FireBullet({ -1, -1 });
		else if (keyState[XK_Up])
			guns[0]->FireBullet({ 0, 1 });
		else if (keyState[XK_Down])
			guns[0]->FireBullet({ 0, -1 });
		else if (keyState[XK_Left])
			guns[0]->FireBullet({ 1, 0 });
		else if (keyState[XK_Right])
			guns[0]->FireBullet({ -1, 0 });

#if IMGUI_DEBUG
		if (keyState[XK_r]) { //up
			cam->moveRelativeToObjVertical(Forward);
		}
		else if (keyState[XK_f]) {  //down 
			cam->moveRelativeToObjVertical(Backward);
		}
#endif

		//PAUSE
		if (keyState[XK_Escape] || keyState[XK_p]) {
			if (pauseTimer < time) {
				time = 0;
			if (!paused)
			{
				paused = true;
				p_physicsManager->UpdateDebug = false;
				ui->setUIMode(PAUSE_SCREEN);
			}
			else
			{
				paused = false;
				p_physicsManager->UpdateDebug = true;
				ui->setUIMode(UI_IN_GAME);
			}
			}
		}
	}

#elif WIN32

	if (!cam->isFreeCam()) {
		if (keyState[GLFW_KEY_W]) {
			players[0]->moveForward(Forward);
		}
		else if (keyState[GLFW_KEY_S])
		{
			players[0]->moveForward(Backward);
		}

		if (keyState[GLFW_KEY_A]) {
			players[0]->moveSideways(Left);
		}
		else if (keyState[GLFW_KEY_D])
		{
			players[0]->moveSideways(Right);
		}

#ifdef _DEBUG
		if (keyState[GLFW_KEY_SPACE])
		{
			players[0]->jump(0.5f);
		}
#endif

		if (keyState[GLFW_KEY_M])
		{
			map->ReBuildLevel(rootFilepath + "res/lvl1.csv", 4.1f,
				rootFilepath + "res/models/ground_rubble/cube.obj",
				rootFilepath + "res/models/wall_brick/cube.obj");
		}

		if (keyState[GLFW_KEY_RIGHT] && keyState[GLFW_KEY_UP])
			guns[0]->FireBullet({ -1, 1 });
		else if (keyState[GLFW_KEY_LEFT] && keyState[GLFW_KEY_UP])
			guns[0]->FireBullet({ 1, 1 });
		else if (keyState[GLFW_KEY_LEFT] && keyState[GLFW_KEY_DOWN])
			guns[0]->FireBullet({ 1, -1 });
		else if (keyState[GLFW_KEY_RIGHT] && keyState[GLFW_KEY_DOWN])
			guns[0]->FireBullet({ -1, -1 });
		else if (keyState[GLFW_KEY_UP])
			guns[0]->FireBullet({ 0, 1 });
		else if (keyState[GLFW_KEY_DOWN])
			guns[0]->FireBullet({ 0, -1 });
		else if (keyState[GLFW_KEY_LEFT])
			guns[0]->FireBullet({ 1, 0 });
		else if (keyState[GLFW_KEY_RIGHT])
			guns[0]->FireBullet({ -1, 0 });

#if IMGUI_DEBUG
		if (keyState[GLFW_KEY_R]) { //up
			cam->moveRelativeToObjVertical(Forward);
		}
		else if (keyState[GLFW_KEY_F]) {  //down 
			cam->moveRelativeToObjVertical(Backward);
		}
#endif
		if (keyState[GLFW_KEY_ESCAPE] || keyState[GLFW_KEY_P]) {
			if (pauseTimer < time) {
				time = 0;
				if (!paused)
				{
					paused = true;
					p_physicsManager->UpdateDebug = false;
					ui->setUIMode(PAUSE_SCREEN);
				}
				else
				{
					paused = false;
					p_physicsManager->UpdateDebug = true;
					ui->setUIMode(UI_IN_GAME);
				}
			}
		}
	}

#endif
}

void Level::RestartLevel()
{
	paused = false;
	p_physicsManager->UpdateDebug = true;
	map->ReBuildLevel(rootFilepath + "res/lvl1.csv", 4.1f,
		rootFilepath + "res/models/ground_rubble/cube.obj",
		rootFilepath + "res/models/wall_brick/cube.obj");
	players[0]->restoreHealth();
	ui->setUIMode(UI_IN_GAME);
}




