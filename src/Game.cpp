#include <fstream>

#include "Game.h"
#include "Bullet.h"
#include "Camera.h"
#include "Level.h"
#include "Map.h"
#include "Shader.h"
#include "GameObject.h"
#include "Renderer.h"
#include "Random.h"
#include "UI.h"

#ifdef RASPBERRY
#include "external/imgui/imgui.h"
#elif WIN32
#include "imgui/imgui.h"
#include <GLFW/glfw3.h>
#endif

Name::Name()
{
	std::ifstream randomNameFile(rootFilepath + "res/names.txt");
	std::ifstream randomSurnameFile(rootFilepath + "res/surnames.txt");

	randomNameFile.seekg(0);
	randomSurnameFile.seekg(0);

	std::string line = "BAD ";
	int lineCounter = 0;
	while (getline(randomNameFile, line))
	{
		lineCounter++;
		if (lineCounter == IRand(1000)) { name = line; }
	}
	linesFile1 = lineCounter;

	lineCounter = 0;
	line = "THING";
	while (getline(randomSurnameFile, line))
	{
		lineCounter++;
		if (lineCounter == IRand(1000)) { surname = line; }
	}
	linesFile2 = lineCounter;
}

void Name::GenerateNewName()
{
	std::ifstream randomNameFile(rootFilepath + "res/names.txt");
	std::ifstream randomSurnameFile(rootFilepath + "res/surnames.txt");

	randomNameFile.seekg(0);
	randomSurnameFile.seekg(0);

	std::string line = "WHAT";
	int lineCounter = 0;
	while (getline(randomNameFile, line))
	{
		lineCounter++;
		if (lineCounter == IRand(linesFile1)) { name = line; }
	}

	lineCounter = 0;
	line = "BRUH";
	while (getline(randomSurnameFile, line))
	{
		lineCounter++;
		if (lineCounter == IRand(linesFile2)) { surname = line; }
	}
}


//TODO:: Build a small gameplay loop so I know what the fuck i need. keepin' it vague - Steven
Game::Game()
{
	gameRunning = true;
}

void Game::OnInit()
{
	renderer = new Renderer;
	camera = new Camera(win_width, win_height);
	ui = new UI(level);
	lightShader = new Shader(rootFilepath + "res/shaders/basicLight");
	level = new GameStateManager(renderer, camera, lightShader, ui, p_physicsWorld, p_physicsmanager, keyState);
	ui->setGMManager(level);
}

void Game::Render()
{
	renderer->Clear(lightShader);
	renderer->resetCounter();
	level->RenderALL();
	level->CheckForNewState();
}

void Game::Update(float deltaTime)
{
	gameRunning = level->gameRunning;
	Input();

	camera->Update(win_width, win_height, deltaTime);
	level->UpdateAll(deltaTime);
	ui->RenderImGUi(win_width, win_height);
}

void Game::OnImGuiRender(float deltaTime, unsigned int frames, float totalTime)
{
#if IMGUI_DEBUG
	//FPS timer for ImGui
	if (totalTime > 1.0f) 
	{
		FPS = static_cast<float>(frames) / totalTime;
		if (counterDTA < deltaTimeArray.size()) { deltaTimeArray[counterDTA] = FPS; counterDTA++; }
		else if (counterDTA == deltaTimeArray.size())
		{
			for (int i = 0; i < deltaTimeArray.size(); i++)
			{
				if (i == deltaTimeArray.size() - 1)
				{
					deltaTimeArray[i] = FPS;
				}
				else {
					deltaTimeArray[i] = deltaTimeArray[i + 1];
					counterDTA--;
				}
			}
		}
	}

	if (ImGui::TreeNode("Global Settings")) {
		ImGui::Text("FPS=%3.4f\n", FPS);  // uncomment to see
		ImGui::Text("deltaTime %f \n", deltaTimeR);  // uncomment to see
		ImGui::Text("Application average %.3f s/frame", deltaTime);
		// Plot some values
		ImGui::PlotHistogram("", &deltaTimeArray[0], static_cast<int>(deltaTimeArray.size()), 0, "FPS", 0, 360, ImVec2(0.f, 60.f));
		renderer->ImGuiLogic();
		ImGui::TreePop();
	}

	camera->UpdateImGui();
	level->UpdateImGuiAll();

	if (ImGui::TreeNode("Random Names")) {
		if(ImGui::Button("Add Name"))
		{
			Name* newName = new Name;;
			names.push_back(newName);
		}
		if (ImGui::Button("Regenerate Names"))
		{
			for (Name* name : names) { name->GenerateNewName(); }
		}
		if (ImGui::Button("Delete Names"))
		{
			
			names.clear();
		}

		for(auto name : names)
		{
			ImGui::Text("%s", name->getFullName().c_str());
		}

		ImGui::TreePop();
	}
#endif
}

void Game::OnShutdown()
{
	delete camera;
	delete renderer;
	delete lightShader;
	delete level;

}

Camera* Game::getCamera() const
{ return camera; }


void Game::Input()
{
#if RASPBERRY

	if (camera->isFreeCam()) {
		if (keyState[XK_Escape] || keyState['q'])
		{
			gameRunning = false;
		}

		if (keyState[XK_W]) {
			camera->moveForward(Forward);
		}
		else if (keyState['s'])
		{
			camera->moveForward(Backward);
		}

		if (keyState['a']) {
			camera->moveSideways(Left);
		}
		else if (keyState['d'])
		{
			camera->moveSideways(Right);
		}

		if (keyState['r']) {
			camera->moveVertical(Forward);
		}
		else if (keyState['f'])
		{
			camera->moveVertical(Backward);
		}

		if (keyState[0xff52]) //up arrow
			camera->moveLookAT(0, Forward);

		if (keyState[0xff54]) //down arrow
			camera->moveLookAT(0, Backward);

		if (keyState[0xff51]) //left arrow
			camera->moveLookAT(1, Left);

		if (keyState[0xff53]) //right arrow
			camera->moveLookAT(1, Right);
	}
#elif WIN32

#ifdef RASPBERRY
	if (keyState[XK_Escape] || keyState['q'])
	{
		gameRunning = false;
	}
#endif

	if (camera->isFreeCam()) {
		if (keyState['W']) {
			camera->moveForward(Forward);
		}
		else if (keyState['S'])
		{
			camera->moveForward(Backward);
		}

		if (keyState['A']) {
			camera->moveSideways(Left);
		}
		else if (keyState['D'])
		{
			camera->moveSideways(Right);
		}

		if (keyState['R']) {
			camera->moveVertical(Forward);
		}
		else if (keyState['F'])
		{
			camera->moveVertical(Backward);
		}

		if (keyState[GLFW_KEY_UP]) //up arrow
			camera->moveLookAT(0, Forward);

		if (keyState[GLFW_KEY_DOWN]) //down arrow
			camera->moveLookAT(0, Backward);

		if (keyState[GLFW_KEY_LEFT]) //left arrow
			camera->moveLookAT(1, Left);

		if (keyState[GLFW_KEY_RIGHT]) //right arrow
			camera->moveLookAT(1, Right);
	}

	//Change cam orientation
	if (keyState[GLFW_KEY_Z]) {

		if (!camera->isFreeCam())
			camera->setFreeCameraMode(true);
		else
			camera->setFreeCameraMode(false);
	}
#endif

}


#ifdef RASPBERRY
void Game::KeyEvent(KeySym key, bool pressed)
{

}

void Game::MouseButtonEvent(MouseButton key, int pressed)
{
	if (mouseState[3]) { printf("scrollign up \n"); }
	if (mouseState[4]) { printf("scrollign down \n"); }
}

void Game::MouseMove(int mousexRel, int mouseyRel)
{
	/*if (mouseState[0]) {
		camera->moveLookAt(-mouseyRel / 2, mousexRel / 2);
	}*/
}

#endif

bool GLLogCall()
{
	while (GLenum error = glGetError())
	{
		printf("[OpenGL Error]: ( %i ) \n", error);
		return false;
	}
	return true;
}
