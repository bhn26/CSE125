#include "MenuState.h"
#include "LobbyState.h"
#include "StateManager.h"
#include "TextRenderer.h"
#include "Window.h"

#include "../Graphics/Shader.h"
#include "ClientGame.h"


CMenuState::CMenuState(CStateManager* pManager) 
  : CGameState(pManager)
{
	// Create the text controls of the menu.
	sprite_renderer = new SpriteRenderer();
	initialized = false;
}

CMenuState::~CMenuState()
{
}

CMenuState* CMenuState::GetInstance(CStateManager* pManager)
{
	static CMenuState Instance(pManager);
	return &Instance;
}

void CMenuState::OnKeyDown(WPARAM wKey)
{
}

void CMenuState::OnClick(int button, double x, double y) {
	GLubyte res[4];
	GLint viewport[4];

	// render selection 
	RenderSelection();

	glGetIntegerv(GL_VIEWPORT, viewport);
	glReadPixels(x, viewport[3] - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &res); 

	switch(res[0]) {
		case 0: printf("None clicked\n"); break;
		case 1: printf("Textbox clicked\n"); break;
		case 2: printf("Button clicked\n");
			// change state
			m_pStateManager->ChangeState(LobbyState::GetInstance(m_pStateManager));
			break;
		default: printf("%d clicked%s\n", res[0]);
	}
}

void CMenuState::RenderSelection() {
	InitTextures();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	////////////////// BACKGROUND//////////////////////////
	float x = Texture::GetWindowCenter(bg->Width());
	float y = Window::height / 2 - bg->Height() / 2;

	////////////// USERNAME TEXTBOX /////////////////////////////////
	sprite_renderer->RenderSelection(1, *textbox, glm::vec2(x + 115, y + 555), glm::vec2(textbox->Width(), textbox->Height()), 0.0f);

	////////////// JOIN BUTTON /////////////////////////////////////
	sprite_renderer->RenderSelection(2, *join, glm::vec2(x + 660, y + 555), glm::vec2(join->Width(), join->Height()), 0.0f);

	glClearColor(0.28f, 0.65f, 0.89f, 1.0f); // reset color
}


void CMenuState::Draw()
{
	InitTextures();

	////////////////// BACKGROUND//////////////////////////
	float x = Texture::GetWindowCenter(bg->Width());
	float y = Window::height / 2 - bg->Height() / 2;
	sprite_renderer->DrawSprite(*bg, glm::vec2(x, y), glm::vec2(bg->Width(), bg->Height()), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	////////////// USERNAME TEXTBOX /////////////////////////////////
	sprite_renderer->DrawSprite(*textbox, glm::vec2(x + 115, y + 555), glm::vec2(textbox->Width(), textbox->Height()), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	char name[20];
	strcpy_s(name, "Player ");
	strcat_s(name, std::to_string(ClientGame::GetClientId()).c_str());
	TextRenderer::RenderText(name, x + 145, y + 585, 1.0f, glm::vec3(0.5f, 0.5f, 0.5f));

	////////////// JOIN BUTTON /////////////////////////////////////
	sprite_renderer->DrawSprite(*join,glm::vec2(x + 660, y + 555), glm::vec2(join->Width(), join->Height()), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
}

void CMenuState::EnterState()
{
}

void::CMenuState::InitTextures() {
	if (!initialized) {
		// Create the different images
		bg = new Texture(GL_TEXTURE_2D, "assets/ui/login/login_bg.png");
		textbox = new Texture(GL_TEXTURE_2D, "assets/ui/login/login_textbox.png");
		join = new Texture(GL_TEXTURE_2D, "assets/ui/login/login_button.png");

		initialized = true;
	}
}
