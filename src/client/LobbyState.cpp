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

	ClientGame::instance()->sendJoinPacket(ClientGame::GetClientId() % 2);
}

LobbyState::~LobbyState()
{
}

LobbyState* LobbyState::GetInstance(CStateManager* pManager)
{
	static LobbyState Instance(pManager);
	return &Instance;
}

void LobbyState::OnKeyDown(WPARAM wKey)
{
}

void LobbyState::OnClick(int button, double x, double y) {
	GLubyte res[4];
	GLint viewport[4];

	// render selection 
	RenderSelection();

	glGetIntegerv(GL_VIEWPORT, viewport);
	glReadPixels(x, viewport[3] - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &res);

	switch (res[0]) {
	case 0: printf("None clicked\n"); break;
	case 1: printf("Logo clicked\n"); break;
	case 2: printf("Start Button clicked\n"); 
		ClientGame::instance()->sendStartPacket();
		m_pStateManager->ChangeState(CPlayState::GetInstance(m_pStateManager)); // start game
		break;
	case 3: printf("Join T0 clicked\n"); 
		ClientGame::instance()->sendJoinPacket(0);
		break;
	case 4: printf("Join T1 clicked\n");
		ClientGame::instance()->sendJoinPacket(1);
		break;
	default: printf("%d clicked%s\n", res[0]);
	}
}

void LobbyState::RenderSelection() {
	InitTextures();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	///////////////////// LOGO ///////////////////////////////////////
	int x = Texture::GetWindowCenter(logo->Width());
	int y = 50;

	sprite_renderer->RenderSelection(1, *logo, glm::vec2(x, y), glm::vec2(logo->Width(), logo->Height()), 0.0f);

	////////////// START BUTTON /////////////////////////////////////
	float btn_width = 200;
	float btn_height = 50;

	x = Texture::GetWindowCenter(btn_width);
	y = Window::height - btn_height - 50;

	sprite_renderer->RenderSelection(2, *start_button, glm::vec2(x, y), glm::vec2(btn_width, btn_height), 0.0f);

	///////////// TEAM 0 //////////////////////////////////////////////
	std::vector<int> team0 = ClientGame::Team0();

	float panel_width = (Window::width / 2) - 20;
	float panel_height = 50;

	x = 10;
	y = 50 + logo->Height() + 50;

	for (int i = 0; i < team0.size(); i++) {
		y = y + panel_height + 10;
	}

	if (std::find(team0.begin(), team0.end(), ClientGame::GetClientId()) == team0.end()) {
		// add join button
		sprite_renderer->RenderSelection(3, *start_button, glm::vec2(x, y), glm::vec2(panel_width, panel_height), 0.0f);
	}

	///////////// TEAM 1 //////////////////////////////////////////////
	std::vector<int> team1 = ClientGame::Team1();
	x = (Window::width / 2) + 10;
	y = 50 + logo->Height() + 50;

	for (int i = 0; i < team1.size(); i++) {
		y = y + panel_height + 10;
	}

	if (std::find(team1.begin(), team1.end(), ClientGame::GetClientId()) == team1.end()) {
		// add join button
		sprite_renderer->RenderSelection(4, *start_button, glm::vec2(x, y), glm::vec2(panel_width, panel_height), 0.0f);
	}

	glClearColor(0.28f, 0.65f, 0.89f, 1.0f); // reset color
}


void LobbyState::Draw()
{
	InitTextures();

	///////////////////// LOGO ///////////////////////////////////////
	int x = Texture::GetWindowCenter(logo->Width());
	int y = 50;

	sprite_renderer->DrawSprite(*logo, glm::vec2(x, y), glm::vec2(logo->Width(), logo->Height()), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	////////////// START BUTTON /////////////////////////////////////
	float btn_width = 200;
	float btn_height = 50;

	x = Texture::GetWindowCenter(btn_width);
	y = Window::height - btn_height - 50;

	sprite_renderer->DrawSprite(*start_button, glm::vec2(x, y), glm::vec2(btn_width, btn_height), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	TextRenderer::RenderText("Start!", x, y, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	///////////// TEAM 0 //////////////////////////////////////////////
	std::vector<int> team0 = ClientGame::Team0();

	float panel_width = (Window::width / 2) - 20;
	float panel_height = 50;

	x = 10;
	y = 50 + logo->Height() + 50;

	for (int i = 0; i < team0.size(); i++) {
		sprite_renderer->DrawSprite(*panel, glm::vec2(x, y), glm::vec2(panel_width, panel_height), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
		char name[20];
		strcpy_s(name, "Player ");
		strcat_s(name, std::to_string(team0.at(i)).c_str());
		TextRenderer::RenderText(name, x, y, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));

		y = y + panel_height + 10;
	}

	if (std::find(team0.begin(), team0.end(), ClientGame::GetClientId()) == team0.end()) {
		// add join button
		sprite_renderer->DrawSprite(*start_button, glm::vec2(x, y), glm::vec2(panel_width, panel_height), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
		TextRenderer::RenderText("+", x, y, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	}

	///////////// TEAM 1 //////////////////////////////////////////////
	std::vector<int> team1 = ClientGame::Team1();
	x = (Window::width / 2) + 10;
	y = 50 + logo->Height() + 50;

	for (int i = 0; i < team1.size(); i++) {
		sprite_renderer->DrawSprite(*panel, glm::vec2(x, y), glm::vec2(panel_width, panel_height), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
		char name[20];
		strcpy_s(name, "Player ");
		strcat_s(name, std::to_string(team1.at(i)).c_str());
		TextRenderer::RenderText(name, x, y, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));

		y = y + panel_height + 10;
	}

	if (std::find(team1.begin(), team1.end(), ClientGame::GetClientId()) == team1.end()) {
		// add join button
		sprite_renderer->DrawSprite(*start_button, glm::vec2(x, y), glm::vec2(panel_width, panel_height), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
		TextRenderer::RenderText("+", x, y, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	}
}

void LobbyState::EnterState()
{
}

void::LobbyState::InitTextures() {
	if (!initialized) {
		// Create the different images
		logo = new Texture(GL_TEXTURE_2D, "assets/ui/logo.png");
		panel = new Texture(GL_TEXTURE_2D, "assets/ui/button_yellow.png");
		start_button = new Texture(GL_TEXTURE_2D, "assets/ui/button_green.png");

		initialized = true;
	}
}
