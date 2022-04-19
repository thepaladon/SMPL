#include "GameState.h"
#include "Level.h"
#include "PhysicsComponent.h"
#include "TitleScreen.h"

GameState::~GameState() { delete currentState; }

void GameState::Update(float dt) {}
void GameState::Render() {}
void GameState::UpdateImGui() {}

void GameState::HandleEvents()
{
	if (currentState != nullptr) {
		currentState->HandleEvents();
		nextState = currentState->getNextState();
	}
}

void GameState::setNextState(gameStates newState)
{
    //If the user doesn't want to exit
    if (nextState != STATE_EXIT)
    {
        //Set the next state
        nextState = newState;
    }
}

void GameState::ChangeState()
{
    //If the state needs to be changed
    if (nextState != STATE_NULL)
    {
        //Delete the current state
        if (nextState != STATE_EXIT)
        {
            delete currentState;
        }
    }
}


GameStateManager::GameStateManager(Renderer* renderer, Camera* cam, Shader* shader, UI* ui,
	btDiscreteDynamicsWorld* physicsWorld, PhysicsManager* physicsManager, bool keyState[65536])
	: renderer(renderer), cam(cam), shader(shader), p_physicsWorld(physicsWorld), p_physicsManager(physicsManager), p_ui(ui)
{
	keyPress = keyState;
	m_gameState = new TitleScreen(renderer, cam, shader, p_ui, physicsWorld, physicsManager, keyPress);
	levelName = "Title Screen";
}

GameStateManager::~GameStateManager()
{
}

void GameStateManager::UpdateAll(float dt)
{
	m_gameState->HandleEvents();

	m_gameState->Update(dt);
}

void GameStateManager::UpdateImGuiAll()
{
	m_gameState->UpdateImGui();
}

void GameStateManager::RenderALL()
{
	m_gameState->Render();
}

void GameStateManager::CheckForNewState()
{
	m_nextState = static_cast<gameStates>(m_gameState->getNextState());

	//If the state needs to be changed
	if (m_nextState != STATE_NULL)
	{
		//Delete the current state
		if (m_nextState != STATE_EXIT)
		{
			delete m_gameState;
			m_gameState = nullptr;
		}
	}

	//Change the state
	switch (m_nextState)
	{
	case STATE_TITLE:
		m_gameState = new TitleScreen(renderer, cam, shader, p_ui, p_physicsWorld, p_physicsManager, keyPress);
		levelName = "Title Screen";
		break;

	case STATE_LEVEL_1:
		m_gameState = new Level(renderer, cam, shader, p_ui, p_physicsWorld, p_physicsManager, keyPress);
		levelName = "Level 1";
		break;
	default:;
	}

	m_gameState->ChangeState();

	//Change the current state ID
	m_stateId = m_nextState;

	//NULL the next state ID
	m_nextState = STATE_NULL;
}

GameState* GameStateManager::getGameState() { return m_gameState; }
