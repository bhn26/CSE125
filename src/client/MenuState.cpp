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
		case 2: printf("Logo clicked\n"); break;
		case 3: printf("Button clicked\n"); 
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

	////////////// USERNAME TEXTBOX /////////////////////////////////
	float x = Texture::GetWindowCenter(Window::width / 2);
	float y = Window::height / 2;

	float tb_width = Window::width / 2;
	float tb_height = 80;

	sprite_renderer->RenderSelection(1, *textbox, glm::vec2(x, y), glm::vec2(tb_width, tb_height), 0.0f);

	///////////////////// LOGO ///////////////////////////////////////
	x = Texture::GetWindowCenter(logo->Width());
	y = y / 2 - logo->Height() / 2;

	sprite_renderer->RenderSelection(2, *logo, glm::vec2(x, y), glm::vec2(logo->Width(), logo->Height()), 0.0f);

	////////////// JOIN BUTTON /////////////////////////////////////
	float btn_width = 200;
	float btn_height = 50;

	x = Texture::GetWindowCenter(btn_width);
	y = Window::height / 2 + tb_height / 2 + 100;

	sprite_renderer->RenderSelection(3, *join, glm::vec2(x, y), glm::vec2(btn_width, btn_height), 0.0f);

	glClearColor(0.28f, 0.65f, 0.89f, 1.0f); // reset color
}


void CMenuState::Draw()
{
	InitTextures();

	////////////// USERNAME TEXTBOX /////////////////////////////////
	float x = Texture::GetWindowCenter(Window::width / 2);
	float y = Window::height / 2;

	float tb_width = Window::width / 2;
	float tb_height = 80;

	sprite_renderer->DrawSprite(*textbox, glm::vec2(x, y), glm::vec2(tb_width, tb_height), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	///////////////////// LOGO ///////////////////////////////////////
	x = Texture::GetWindowCenter(logo->Width());
	y = y/2 - logo->Height()/2;

	sprite_renderer->DrawSprite(*logo, glm::vec2(x, y), glm::vec2(logo->Width(), logo->Height()), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	////////////// JOIN BUTTON /////////////////////////////////////
	float btn_width = 200;
	float btn_height = 50;

	x = Texture::GetWindowCenter(btn_width);
	y = Window::height/2 + tb_height/2 + 100;

	sprite_renderer->DrawSprite(*join, glm::vec2(x, y), glm::vec2(btn_width, btn_height), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	TextRenderer::RenderText("Join", x, y, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
}

void CMenuState::EnterState()
{
}

void::CMenuState::InitTextures() {
	if (!initialized) {
		// Create the different images
		logo = new Texture(GL_TEXTURE_2D, "assets/ui/logo.png");
		textbox = new Texture(GL_TEXTURE_2D, "assets/ui/textbox.png");
		join = new Texture(GL_TEXTURE_2D, "assets/ui/button_green.png");

		initialized = true;
	}
}
