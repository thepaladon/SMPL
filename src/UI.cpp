
// ReSharper disable CppClangTidyClangDiagnosticImplicitIntFloatConversion
#include "UI.h"

#include "Bullet.h"

#ifdef RASPBERRY
#include "external/imgui/imgui.h"
#elif WIN32
#include "imgui/imgui.h"
#endif


UI::UI(GameStateManager* stateManager)
	:state(stateManager) {}

void UI::setPlayerValues(int* p_playerHealth, int* p_playerMaxHealth)
{
	playerHealth = p_playerHealth;
	playerMaxHealth = p_playerMaxHealth;
}

void UI::RenderImGUi(int screenX, int screenY)
{
#if  !IMGUI_DEBUG
	const float scrX = static_cast<float>(screenX);
	const float scrY = static_cast<float>(screenY);

	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(scrX, scrY));

	ImGui::Begin("UI", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBackground);

    ImGui::SetWindowFontScale(1.5f);
    ImGui::PushStyleColor(0, ImVec4(1.f, 1.f, 1.f, 1.f));
    constexpr int buttonSizeY = 175;
    constexpr int buttonSizeX = 350;
    glm::vec2 smallButton = { buttonSizeX / 3.f, buttonSizeY / 3.f };

    switch (titleScreen) {

    case TITLE_SCREEN:
        ImGui::SetWindowFontScale(5.f);
        ImGui::SetCursorPos(ImVec2(scrX / 2 - 250, scrY / 2 - 350));
        ImGui::Text("Gauntlet Gaming");

        ImGui::SetWindowFontScale(2.5f);
        ImGui::SetCursorPos(ImVec2(scrX / 2 - buttonSizeX / 2, scrY / 2 - buttonSizeY / 2 - (buttonSizeY + 30) + 20));
        if (ImGui::Button("Start Game!", ImVec2(buttonSizeX, buttonSizeY)))
        {
            titleScreen = CHOOSE_CHARACTER;
        }

        ImGui::SetWindowFontScale(2.5f);
        ImGui::SetCursorPos(ImVec2(scrX / 2 - buttonSizeX / 2, scrY / 2 - buttonSizeY / 2 + 20));
        if (ImGui::Button("Instructions!", ImVec2(buttonSizeX, buttonSizeY)))
        {
            titleScreen = INSTRUCTIONS;
        }

        ImGui::SetWindowFontScale(2.5f);
        ImGui::SetCursorPos(ImVec2(scrX / 2 - buttonSizeX / 2, scrY / 2 - buttonSizeY / 2 + (buttonSizeY + 30) + 20));
        if (ImGui::Button("Quit Game!", ImVec2(buttonSizeX, buttonSizeY)))
        {
            state->gameRunning = false;
        }

        ImGui::SetWindowFontScale(1.f);

        ImGui::SetCursorPos(ImVec2(5.f, scrY - 15.f));
        ImGui::Text("BUAS Uni, Year 1, Block C");

        ImGui::SetCursorPos(ImVec2(5.f, scrY - 25.f));
        ImGui::Text("Made By: Angel Angelov");
        break;

    case INSTRUCTIONS:
        ImGui::SetWindowFontScale(2.5f);
        ImGui::SetCursorPos(ImVec2(scrX / 2 - 250, scrY / 2 - 200));
        ImGui::Text("Instruction");

        ImGui::Text("Movement Keys: W, A, S, D");
        ImGui::Text("Shooting keys: Arrow Keys");
        ImGui::Text("Press P or ESC to bring up the Pause");
        ImGui::NewLine();

        ImGui::Text("GOAL:");
        ImGui::Text("Get through the level and find the exit!");

        ImGui::SetWindowFontScale(2.f);
        ImGui::SetCursorPos(ImVec2(scrX / 2 - buttonSizeX / 2, scrY / 2 - buttonSizeY / 2 + buttonSizeY + 30));
        if (ImGui::Button("Back to Title Screen", ImVec2(buttonSizeX, buttonSizeY)))
        {
            titleScreen = TITLE_SCREEN;
        }

        ImGui::SetWindowFontScale(1.f);
        ImGui::SetCursorPos(ImVec2(5.f, scrY - 15.f));
        ImGui::Text("BUAS Uni, Year 1, Block C");

        ImGui::SetCursorPos(ImVec2(5.f, scrY - 25.f));
        ImGui::Text("Made By: Angel Angelov");

        break;

    case CHOOSE_CHARACTER:

        ImGui::SetWindowFontScale(2.5f);
        ImGui::SetCursorPos(ImVec2(scrX / 2 - 250, scrY / 2 - 180));
        ImGui::Text("Choose your fighter:");

        ImGui::SetWindowFontScale(2.f);
        ImGui::SetCursorPos(ImVec2(scrX / 2 - 250, scrY / 2 - 200 + (smallButton.y + 10) * 1));
        if (ImGui::Button("Wizard!", ImVec2(smallButton.x, smallButton.y)))
        {
            playerSelectedCharacter = PlayerClass::Mage;
            state->getGameState()->setNextState(STATE_LEVEL_1);
            titleScreen = UI_IN_GAME;
        }

        ImGui::SetCursorPos(ImVec2(scrX / 2 - 250, scrY / 2 - 200 + (smallButton.y + 10) * 2));
        if (ImGui::Button("Elf!", ImVec2(smallButton.x, smallButton.y)))
        {
            playerSelectedCharacter = PlayerClass::Elf;
            state->getGameState()->setNextState(STATE_LEVEL_1);
            titleScreen = UI_IN_GAME;
        }

        ImGui::SetCursorPos(ImVec2(scrX / 2 - 250, scrY / 2 - 200 + (smallButton.y + 10) * 3));
        if (ImGui::Button("Warrior!", ImVec2(smallButton.x, smallButton.y)))
        {
            playerSelectedCharacter = PlayerClass::Warrior;
            state->getGameState()->setNextState(STATE_LEVEL_1);
            titleScreen = UI_IN_GAME;
        }

        ImGui::SetCursorPos(ImVec2(scrX / 2 - 250, scrY / 2 - 200 + (smallButton.y + 10) * 4));
        if (ImGui::Button("Valkyre!", ImVec2(smallButton.x, smallButton.y)))
        {
            playerSelectedCharacter = PlayerClass::Valkyre;
            state->getGameState()->setNextState(STATE_LEVEL_1);
            titleScreen = UI_IN_GAME;
        }

        break;
    case UI_IN_GAME:
        ImGui::SetCursorPos(ImVec2(5, scrY - 55));
        ImGui::ProgressBar(static_cast<float>(*playerHealth) / static_cast<float>(*playerMaxHealth), ImVec2(200, 60), "Health");

        break;

    case PAUSE_SCREEN:

        ImGui::SetWindowFontScale(5.f);
        ImGui::SetCursorPos(ImVec2(scrX / 2 - 175, scrY / 2 - 250));
        ImGui::Text("PAUSED");

        ImGui::SetWindowFontScale(3.5f);
        ImGui::SetCursorPos(ImVec2(scrX / 2 - buttonSizeX / 2, scrY / 2 - buttonSizeY / 2));
        if (ImGui::Button("Retry Level", ImVec2(buttonSizeX, buttonSizeY)))
        {
            restartLevel = true;
            titleScreen = UIMode::UI_IN_GAME;
        }

        ImGui::SetCursorPos(ImVec2(scrX / 2 - buttonSizeX / 2, scrY / 2 - buttonSizeY / 2 + buttonSizeY + 30));
        if (ImGui::Button("Exit to Title", ImVec2(buttonSizeX, buttonSizeY)))
        {
            state->getGameState()->setNextState(STATE_TITLE);
            titleScreen = UIMode::TITLE_SCREEN;
        }

        break;

    case WIN_SCREEN:
        ImGui::SetWindowFontScale(3.5f);
        ImGui::SetCursorPos(ImVec2(scrX / 2 - 300, scrY / 2 - 250));
        ImGui::Text("You Completed The Level!");

        ImGui::SetWindowFontScale(3.5f);
        ImGui::SetCursorPos(ImVec2(scrX / 2 - buttonSizeX / 2, scrY / 2 - buttonSizeY / 2));
        if (ImGui::Button("Retry Level", ImVec2(buttonSizeX, buttonSizeY)))
        {
            restartLevel = true;
            titleScreen = UIMode::UI_IN_GAME;
        }

        ImGui::SetCursorPos(ImVec2(scrX / 2 - buttonSizeX / 2, scrY / 2 - buttonSizeY / 2 + buttonSizeY + 30));
        if (ImGui::Button("Exit to Title", ImVec2(buttonSizeX, buttonSizeY)))
        {
            state->getGameState()->setNextState(STATE_TITLE);
            titleScreen = UIMode::TITLE_SCREEN;
        }

        break;
    case GAME_OVER:
        ImGui::SetWindowFontScale(5.f);
        ImGui::SetCursorPos(ImVec2(scrX / 2 - 175, scrY / 2 - 250));
        ImGui::Text("Game Over");

        ImGui::SetWindowFontScale(3.5f);
        ImGui::SetCursorPos(ImVec2(scrX / 2 - buttonSizeX / 2, scrY / 2 - buttonSizeY / 2));
        if (ImGui::Button("Retry Level", ImVec2(buttonSizeX, buttonSizeY)))
        {
            restartLevel = true;
            titleScreen = UIMode::UI_IN_GAME;
        }

        ImGui::SetCursorPos(ImVec2(scrX / 2 - buttonSizeX / 2, scrY / 2 - buttonSizeY / 2 + buttonSizeY + 30));
        if (ImGui::Button("Exit to Title", ImVec2(buttonSizeX, buttonSizeY)))
        {
            state->getGameState()->setNextState(STATE_TITLE);
            titleScreen = UIMode::TITLE_SCREEN;
        }
        break;
        }

        ImGui::PopStyleColor(1);
        ImGui::SetWindowFontScale(1.f);
        ImGui::SetCursorPos(ImVec2(5.f, 0.f));
        ImGui::Text("Current Game State: %s", state->getName().c_str());

        ImGui::End();
#else

    if (ImGui::TreeNode("State Manager"))
    {
        if (ImGui::Button("Title"))
        {
            state->getGameState()->setNextState(STATE_TITLE);
            titleScreen = UIMode::TITLE_SCREEN;
        }

        if (ImGui::Button("Game"))
        {
            state->getGameState()->setNextState(STATE_LEVEL_1);
            titleScreen = UIMode::UI_IN_GAME;
        }

        ImGui::TreePop();
    }

#endif
    
}
