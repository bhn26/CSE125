#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "PlayState.h"
#include "StateManager.h"
#include "MenuState.h"
#include <sstream>

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
	Scene::Instance()->GetPlayer()->ProcessMouseMovement(xoffset, yoffset);
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

		// update scores
		std::vector<std::shared_ptr<Player>> players = Scene::Instance()->GetPlayers();
	
		scores[0] = 0; // clear score
		std::vector<int> team0 = ClientGame::Team0();
		for (int i = 0; i < team0.size(); i++) {
			scores[0] += players.at(team0.at(i))->GetScore();
		}

		scores[1] = 0; //clear score
		std::vector<int> team1 = ClientGame::Team1();
		for (int i = 0; i < team1.size(); i++) {
			scores[1] += players.at(team1.at(i))->GetScore();
		}
	}
}

void CPlayState::Draw()  
{ 
	TextRenderer::RenderText("Team 0: ", 50, 50, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	TextRenderer::RenderText("Team 0: ", Window::width - 50, 50, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	/*stringstream ssScore;
	ssScore << m_ulCurrentScore;
	m_pScoreControl->SetText(ssScore.str());
	m_pScoreControl->Draw();*/

	if (!m_bGameOver)
	{
		Scene::Instance()->Draw();
	}

}

