#include "MenuState.h"
#include "PlayState.h"
#include "StateManager.h"
#include "TextRenderer.h"
#include "Window.h"

#include "../Graphics/Shader.h"
#include "ClientGame.h"


CMenuState::CMenuState(CStateManager* pManager) 
  : CGameState(pManager), m_iCurrentSelection(0), m_pCurrentGame(NULL)
{
	// Create the different images
	/*panel = new Texture(GL_TEXTURE_2D, "assets/ui/panel.png");
	logo = new Texture(GL_TEXTURE_2D, "assets/ui/logo.png");
	textbox = new Texture(GL_TEXTURE_2D, "assets/ui/textbox.png");
	join = new Texture(GL_TEXTURE_2D, "assets/ui/button_green.png");*/

	// Create the text controls of the menu.
	sprite_renderer = new SpriteRenderer();
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
	switch (wKey)
	{
	case VK_DOWN:
		SelectionDown();
		break;
	case VK_UP:
		SelectionUp();
		break;
	case VK_RETURN:
		SelectionChosen();
		break;
	}
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
		case 3: printf("Button clicked\n"); break;
		default: printf("%d clicked%s\n", res[0]);
	}
}

void CMenuState::RenderSelection() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	////////////// USERNAME TEXTBOX /////////////////////////////////
	Texture textbox = Texture(GL_TEXTURE_2D, "assets/ui/textbox.png");

	float x = Texture::GetWindowCenter(Window::width / 2);
	float y = Window::height / 2;

	float tb_width = Window::width / 2;
	float tb_height = 80;

	sprite_renderer->RenderSelection(1, textbox, glm::vec2(x, y), glm::vec2(tb_width, tb_height), 0.0f);

	///////////////////// LOGO ///////////////////////////////////////
	Texture logo = Texture(GL_TEXTURE_2D, "assets/ui/logo.png");

	x = Texture::GetWindowCenter(logo.Width());
	y = y / 2 - logo.Height() / 2;

	sprite_renderer->RenderSelection(2, logo, glm::vec2(x, y), glm::vec2(logo.Width(), logo.Height()), 0.0f);

	////////////// JOIN BUTTON /////////////////////////////////////

	Texture join = Texture(GL_TEXTURE_2D, "assets/ui/button_green.png");

	float btn_width = 200;
	float btn_height = 50;

	x = Texture::GetWindowCenter(btn_width);
	y = Window::height / 2 + tb_height / 2 + 100;

	sprite_renderer->RenderSelection(3, join, glm::vec2(x, y), glm::vec2(btn_width, btn_height), 0.0f);

	glClearColor(0.28f, 0.65f, 0.89f, 1.0f); // reset color
	glfwSwapBuffers(ClientGame::instance()->window); // to debug that the selection buffer looks like

}


void CMenuState::Draw()
{
	////////////// USERNAME TEXTBOX /////////////////////////////////
	Texture textbox = Texture(GL_TEXTURE_2D, "assets/ui/textbox.png");

	float x = Texture::GetWindowCenter(Window::width / 2);
	float y = Window::height / 2;

	float tb_width = Window::width / 2;
	float tb_height = 80;

	sprite_renderer->DrawSprite(textbox, glm::vec2(x, y), glm::vec2(tb_width, tb_height), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	///////////////////// LOGO ///////////////////////////////////////
	Texture logo = Texture(GL_TEXTURE_2D, "assets/ui/logo.png");

	x = Texture::GetWindowCenter(logo.Width());
	y = y/2 - logo.Height()/2;

	sprite_renderer->DrawSprite(logo, glm::vec2(x, y), glm::vec2(logo.Width(), logo.Height()), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	////////////// JOIN BUTTON /////////////////////////////////////

	Texture join = Texture(GL_TEXTURE_2D, "assets/ui/button_green.png");

	float btn_width = 200;
	float btn_height = 50;

	x = Texture::GetWindowCenter(btn_width);
	y = Window::height/2 + tb_height/2 + 100;

	sprite_renderer->DrawSprite(join, glm::vec2(x, y), glm::vec2(btn_width, btn_height), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	//TextRenderer::RenderText("Join", x, y, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
}

void CMenuState::EnterState()
{
	// Checks whether there is a current game active
	m_iCurrentSelection = 0;
	/*if (!m_pCurrentGame || m_pCurrentGame->IsGameOver())
		//m_pResumeGameText->SetTextColor(0.5,0.5,0.5);
	else
		//m_pResumeGameText->SetTextColor(1.0,1.0,1.0);*/
}

void CMenuState::SelectionUp()
{
	m_iCurrentSelection--;
	if (m_iCurrentSelection==-1)
		m_iCurrentSelection = 3;

	// If there is no current game, we should skip
	// the "Resume game" item.
	if (m_iCurrentSelection==1) 
	{
		if (!m_pCurrentGame || m_pCurrentGame->IsGameOver())
			m_iCurrentSelection--;
	}
}

void CMenuState::SelectionDown()
{
	m_iCurrentSelection++;
	if (m_iCurrentSelection==4)
		m_iCurrentSelection = 0;

	// If there is no current game, we should skip
	// the "Resume game" item.
	if (m_iCurrentSelection==1) 
	{
		if (!m_pCurrentGame || m_pCurrentGame->IsGameOver())
			m_iCurrentSelection++;
	}
}

void CMenuState::SelectionChosen()
{
	switch (m_iCurrentSelection)
	{
	case 0:
		if (!m_pCurrentGame)
			m_pCurrentGame = CPlayState::GetInstance(m_pStateManager);
		m_pCurrentGame->Reset();
		ChangeState(m_pCurrentGame);
		break;

	case 1:
		if (m_pCurrentGame && !m_pCurrentGame->IsGameOver())
			ChangeState(m_pCurrentGame);
		break;

	case 2:
		//ChangeState(CHighScoreState::GetInstance(m_pStateManager));
		break;

	case 3:
		PostQuitMessage(0);
		break;
	}
}
