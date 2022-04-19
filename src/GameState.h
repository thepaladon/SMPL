#pragma once
#include <string>
#include <glm/fwd.hpp>

class UI;
class PhysicsManager;
class ObjectManager;
class Shader;
class Renderer;
class btDiscreteDynamicsWorld;
class Camera;

enum gameStates : glm::uint8_t
{
    STATE_NULL,
    STATE_TITLE,
    STATE_LEVEL_1,
    STATE_EXIT,
};

class GameState 
{
public:
    virtual void Update(float dt);
    virtual void Render();
    virtual void UpdateImGui();
    virtual void HandleEvents();
    virtual ~GameState();

    //State status manager
    void setNextState(gameStates newState);
    //State changer
    void ChangeState();

    int getNextState() const { return nextState; }
    void setDeltaTime(float tick_deltaTime) { deltaTime = tick_deltaTime; }

    //Game state object
    GameState* currentState = nullptr;
    
protected:

    //State variables
    int stateID = STATE_NULL;
    int nextState = STATE_NULL;
    float deltaTime = 0;
};


class GameStateManager
{
public:
	GameStateManager(Renderer* renderer, Camera* cam, Shader* shader, UI* ui, btDiscreteDynamicsWorld* physicsWorld, PhysicsManager* physicsManager, bool keyState[65536]);
	~GameStateManager();

	void UpdateAll(float dt);
	void UpdateImGuiAll();
    void RenderALL();
    void CheckForNewState();

    GameState* getGameState();
    std::string getName() { return levelName; }

	bool gameRunning = true;
private:
	GameState* m_gameState = nullptr;

	bool m_paused = false;
	float m_timeValue = 1;

	//State variables
	gameStates m_stateId = STATE_NULL;
	gameStates m_nextState = STATE_NULL;
    std::string levelName;

	//References
    bool* keyPress;
    UI* p_ui;
	btDiscreteDynamicsWorld* p_physicsWorld;
    PhysicsManager* p_physicsManager;
    Renderer* renderer;
    Camera* cam;
    Shader* shader;

};