#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "PlayState.h"
#include "StateManager.h"
#include "MenuState.h"
#include <sstream>
#include <math.h>

#include "../Graphics/Scene.h"
#include "Player.h"
#include "client\ClientGame.h"
#include "client\TextRenderer.h"

using namespace std;

CPlayState::CPlayState(CStateManager* pManager)
 : CGameState(pManager), m_bGameOver(false)
{
	//glfwSetInputMode(ClientGame::instance()->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

CPlayState::~CPlayState()
{
	
}

CPlayState* CPlayState::GetInstance(CStateManager* pManager)
{
	static CPlayState Instance(pManager);
	return &Instance;
}

void CPlayState::Reset()
{
	m_bGameOver = false;
}

void CPlayState::OnMouseMove(float xoffset, float yoffset) {
	Player * player = Scene::Instance()->GetPlayer();
	if (player != NULL) {
		player->ProcessMouseMovement(xoffset, yoffset);
	}
}

void CPlayState::OnClick(int button, double x, double y) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && !Window::mouseCaptured)
	{
		Window::mouseCaptured = true;
		Window::firstMouse = true;
		glfwSetInputMode(ClientGame::instance()->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && Window::mouseCaptured)
	{
		Window::mouseCaptured = false;
		glfwSetInputMode(ClientGame::instance()->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void CPlayState::OnKeyDown(WPARAM wKey)
{
	switch (wKey)
	{
	case VK_UP:
		if (!m_bGameOver)
			//m_pMatrix->ShapeRotate();
		break;
	case VK_DOWN:
		if (!m_bGameOver)	
			//m_pMatrix->ShapeDown();
		break;
	case VK_LEFT:
		if (!m_bGameOver)
			//m_pMatrix->ShapeLeft();
		break;
	case VK_RIGHT:
		if (!m_bGameOver)
			//m_pMatrix->ShapeRight();
		break;
	case VK_ESCAPE:
		ChangeState(CMenuState::GetInstance(m_pStateManager));
		break;
	case VK_RETURN:
		if (m_bGameOver)
		{
			
		}
	}
}

void CPlayState::Update(DWORD dwCurrentTime)
{
	if (!m_bGameOver)
	{
		// update scene
		Scene::Instance()->Update();
	}
}

void CPlayState::Draw()  
{ 
	if (!m_bGameOver)
	{
		Scene::Instance()->Draw();
	}
	// TEAM SCORES
	char score0[10];
	strcpy_s(score0, "Team 0: ");
	strcat_s(score0, std::to_string(ClientGame::instance()->GetScores()[0]).c_str());
	TextRenderer::RenderText(score0, 25, 25, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	char score1[10];
	strcpy_s(score1, "Team 1: ");
	strcat_s(score1, std::to_string(ClientGame::instance()->GetScores()[1]).c_str());
	TextRenderer::RenderText(score1, Window::width - 175, 25, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	// NEUTRAL EGGS
	int n = 2 * (ClientGame::Team0().size() + ClientGame::Team1().size());
	n = n - ClientGame::instance()->GetScores()[0] - ClientGame::instance()->GetScores()[1];
	char neutral[20];
	strcpy_s(neutral, "Neutral: ");
	strcat_s(neutral, std::to_string(n).c_str());
	TextRenderer::RenderText(neutral, Window::width/2 - 175, 25, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	// SELF NUMBER OF EGGS
	if (Scene::Instance()->GetPlayer() != NULL) {
		char score[20];
		strcpy_s(score, "Eggs Collected: ");
		strcat_s(score, std::to_string(Scene::Instance()->GetPlayer()->GetScore()).c_str());
		TextRenderer::RenderText(score, 25, Window::height - 50, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	}

}



