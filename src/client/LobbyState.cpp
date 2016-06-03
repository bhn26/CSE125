#include "LobbyState.h"
#include "PlayState.h"
#include "StateManager.h"
#include "TextRenderer.h"
#include "Window.h"

#include "../Graphics/Shader.h"
#include "ClientGame.h"

#include <string.h>


LobbyState::LobbyState(CStateManager* pManager)
	: CGameState(pManager)
{
	// Create the text controls of the menu.
	sprite_renderer = new SpriteRenderer();
	initialized = false;
	loading = false;

	ClientGame::instance()->sendJoinPacket(ClientGame::GetClientId() % 2);
}

LobbyState::~LobbyState()
{
    delete sprite_renderer;
    delete bg;
    delete start_button;

    delete table_t1;
    delete table_t2;

    delete join;
    delete join_disabled;

	delete load_screen;
}

LobbyState* LobbyState::GetInstance(CStateManager* pManager)
{
	static LobbyState Instance(pManager);
	return &Instance;
}

void LobbyState::OnKeyDown(WPARAM wKey)
{
}

void LobbyState::OnClick(int button, int action, double x, double y) {
	GLubyte res[4];
	GLint viewport[4];

	// render selection 
	RenderSelection();

	glGetIntegerv(GL_VIEWPORT, viewport);
	glReadPixels(x, viewport[3] - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &res);
	if (action == GLFW_PRESS) {
        SoundsHandler::SoundOptions options;
        options._isRelativeToListener = true;
		switch (res[0]) {
            case 0: printf("None clicked\n"); break;
            case 1: printf("Start Button clicked\n");
                ClientGame::instance()->PlaySound("Button_Click", options);
                ClientGame::instance()->sendStartPacket();
                break;
            case 2: printf("Join T0 clicked\n");  // team 1
                ClientGame::instance()->PlaySound("Button_Click", options);
                ClientGame::instance()->sendJoinPacket(0);
                break;
            case 3: printf("Join T1 clicked\n"); // team 2
                ClientGame::instance()->PlaySound("Button_Click", options);
                ClientGame::instance()->sendJoinPacket(1);
                break;
            default: printf("%d clicked%s\n", res[0]);
                ClientGame::instance()->PlaySound("Button_Click", options);
        }
	}
}

void LobbyState::RenderSelection() {
	if (!loading) {
		InitTextures();

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		////////////////// BACKGROUND//////////////////////////
		float x = (float)Texture::GetWindowCenter(bg->Width());
		float y = Window::height / 2.0f - bg->Height() / 2.0f;

		////////////// START BUTTON /////////////////////////////////////
		sprite_renderer->RenderSelection(1, *start_button, glm::vec2(x + 1000, y + 55), glm::vec2(start_button->Width(), start_button->Height()), 0.0f);

		//////////////// JOIN BUTTONS ////////////////////////////////
		float ty = y + 255;
		float tx1 = x + (bg->Width() / 2) - (table_t1->Width() + 20);
		float tx2 = tx1 + table_t1->Width() + 40;

		// adjust tx, ty for transparent pixels
		tx1 += 4;
		tx2 += 4;
		ty += 5;

		std::vector<int> team0 = ClientGame::Team0();
		if (std::find(team0.begin(), team0.end(), ClientGame::GetClientId()) == team0.end()) {
			// add join button for t1
			sprite_renderer->RenderSelection(2, *join, glm::vec2(tx1, ty + 390), glm::vec2(join->Width(), join->Height()), 0.0f);
		}
		else { // t2
			sprite_renderer->RenderSelection(3, *join, glm::vec2(tx2, ty + 390), glm::vec2(join->Width(), join->Height()), 0.0f);
		}


		glClearColor(0.28f, 0.65f, 0.89f, 1.0f); // reset color
	}
}


void LobbyState::Draw()
{
	if (!loading) {
		InitTextures();

		////////////////// BACKGROUND//////////////////////////
		float x = (float)Texture::GetWindowCenter(bg->Width());
		float y = Window::height / 2.0f - bg->Height() / 2.0f;
		sprite_renderer->DrawSprite(*bg, glm::vec2(x, y), glm::vec2(bg->Width(), bg->Height()), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

		////////////// START BUTTON /////////////////////////////////////
		sprite_renderer->DrawSprite(*start_button, glm::vec2(x + 1000, y + 55), glm::vec2(start_button->Width(), start_button->Height()), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

		///////////////// TABLES ////////////////////////////////////////
		float ty = y + 255;

		float tx1 = x + (bg->Width() / 2) - (table_t1->Width() + 20);
		sprite_renderer->DrawSprite(*table_t1, glm::vec2(tx1, ty), glm::vec2(table_t1->Width(), table_t1->Height()), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

		float tx2 = tx1 + table_t1->Width() + 40;
		sprite_renderer->DrawSprite(*table_t2, glm::vec2(tx2, ty), glm::vec2(table_t2->Width(), table_t2->Height()), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

		// adjust tx, ty for transparent pixels
		tx1 += 4;
		tx2 += 4;
		ty += 5;

		///////////// TEAM 0 //////////////////////////////////////////////
		std::vector<int> team0 = ClientGame::Team0();
		x = tx1 + 6;
		y = ty + 90 + 6; // skip header

		for (int i = 0; i < team0.size(); i++) {
			TextRenderer::RenderText(ClientGame::instance()->GetName(team0.at(i)).c_str(), x, y, 1.0f, glm::vec3(0.5f, 0.5f, 0.5f));

			y = y + 60; // row height
		}

		if (std::find(team0.begin(), team0.end(), ClientGame::GetClientId()) == team0.end()) {
			// add join button
			sprite_renderer->DrawSprite(*join, glm::vec2(tx1, ty + 390), glm::vec2(join->Width(), join->Height()), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
		}
		else {
			sprite_renderer->DrawSprite(*join_disabled, glm::vec2(tx1, ty + 390), glm::vec2(join->Width(), join->Height()), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
		}

		///////////// TEAM 1 //////////////////////////////////////////////
		std::vector<int> team1 = ClientGame::Team1();
		x = tx2 + 6;
		y = ty + 90 + 6; // skip header

		for (int i = 0; i < team1.size(); i++) {
			TextRenderer::RenderText(ClientGame::instance()->GetName(team1.at(i)).c_str(), x, y, 1.0f, glm::vec3(0.5f, 0.5f, 0.5f));

			y = y + 60; // row height
		}

		if (std::find(team1.begin(), team1.end(), ClientGame::GetClientId()) == team1.end()) {
			// add join button
			sprite_renderer->DrawSprite(*join, glm::vec2(tx2, ty + 390), glm::vec2(join->Width(), join->Height()), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
		}
		else {
			sprite_renderer->DrawSprite(*join_disabled, glm::vec2(tx2, ty + 390), glm::vec2(join->Width(), join->Height()), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
		}
	} else {
		ShowLoadingScreen();
	}
}

void LobbyState::EnterState()
{
}

void LobbyState::LeaveState()
{
    ClientGame::instance()->StopMenuSound();
}

void::LobbyState::InitTextures() {
	if (!initialized) {
		// Create the different images
		bg = new Texture(GL_TEXTURE_2D, "assets/ui/lobby/lobby_bg.png");
		start_button = new Texture(GL_TEXTURE_2D, "assets/ui/lobby/lobby_start.png");

		table_t1 = new Texture(GL_TEXTURE_2D, "assets/ui/lobby/lobby_t1.png");
		table_t2 = new Texture(GL_TEXTURE_2D, "assets/ui/lobby/lobby_t2.png");

		join = new Texture(GL_TEXTURE_2D, "assets/ui/lobby/lobby_join.png");
		join_disabled = new Texture(GL_TEXTURE_2D, "assets/ui/lobby/lobby_join_disabled.png");

		load_screen = new Texture(GL_TEXTURE_2D, "assets/ui/loading/instructions.png");
		initialized = true;
	}
}

void LobbyState::ShowLoadingScreen() {
	int x = Texture::GetWindowCenter(load_screen->Width());
	int y = Window::height / 2 - load_screen->Height() / 2;

	sprite_renderer->DrawSprite(*load_screen, glm::vec2(x, y), glm::vec2(load_screen->Width(), load_screen->Height()), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	//glfwSwapBuffers(ClientGame::instance()->window);
}
