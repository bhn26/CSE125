#include "MenuState.h"
#include "PlayState.h"
#include "StateManager.h"
#include "TextRenderer.h"
#include "Window.h"

#include "../Graphics/Shader.h"
#include "../Graphics/Texture.h"


CMenuState::CMenuState(CStateManager* pManager) 
  : CGameState(pManager), m_iCurrentSelection(0), m_pCurrentGame(NULL)
{
	// Create the different images
	/*m_pBackgroundImg = CImage::CreateImage("bin/MainBackground.png",TRectanglei(0,600,0,800));
	m_pTitleImg = CImage::CreateImage("bin/MenuTitle.png",TRectanglei(0,600,0,800));
	m_pItemBckgndNormal = CImage::CreateImage("bin/MenuItems.png",TRectanglei(0,57,0,382));
	m_pItemBckgndSelected = CImage::CreateImage("bin/MenuItems.png",TRectanglei(58,114,0,382));*/

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


void CMenuState::Draw()
{
	/*m_pBackgroundImg->BlitImage();
	m_pTitleImg->BlitImage();
	// Draw the menu item backgrounds
	for (int i=0;i<4;i++)
	{
		if (i==m_iCurrentSelection)
			m_pItemBckgndSelected->BlitImage(209,150+i*100);
		else
			m_pItemBckgndNormal->BlitImage(209,150+i*100);
	}*/

	////////////////// Background ///////////////////////////////////
	Texture panel = Texture(GL_TEXTURE_2D, "assets/ui/sprites/blue_panel.png");
	sprite_renderer->DrawSprite(panel, glm::vec2(0, 0), glm::vec2(Window::width, Window::height), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	///////////////////// LOGO ///////////////////////////////////////
	Texture logo = Texture(GL_TEXTURE_2D, "assets/ui/logo.png");

	int x = Window::width/2 - logo.Width()/2;
	int y = 20;

	sprite_renderer->DrawSprite(logo, glm::vec2(x, y), glm::vec2(logo.Width(), logo.Height()), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	////////////// USERNAME TEXTBOX /////////////////////////////////

	////////////// JOIN BUTTON /////////////////////////////////////

	// draw text
	TextRenderer::RenderText("This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
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
