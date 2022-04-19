#pragma once
#pragma warning(disable : 4100) //disabled since those parameters are used in the Release build

#include "GameState.h"
#include "glm/glm.hpp"

enum class PlayerClass;

enum UIMode
{
	TITLE_SCREEN,
	CHOOSE_CHARACTER,
	INSTRUCTIONS,
	UI_IN_GAME,
	PAUSE_SCREEN,
	WIN_SCREEN,
	GAME_OVER,
};

class UI
{
public:
	UI(GameStateManager* stateManager);
	~UI() = default;

	void setGMManager(GameStateManager* stateManager) { state = stateManager; }
	void setPlayerValues(int* p_playerHealth, int* p_playerMaxHealth);
	void RenderImGUi(int screenX, int screenY);
	UIMode getUIMode() const { return titleScreen; }
	void setUIMode(UIMode newValue) { titleScreen = newValue; }

	PlayerClass playerSelectedCharacter{0};
	bool restartLevel = false;
private:
	UIMode titleScreen = UIMode::TITLE_SCREEN;
	GameStateManager* state;
	int* playerHealth;
	int* playerMaxHealth;

};
