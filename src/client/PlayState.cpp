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
 : CGameState(pManager), show_scoreboard(false)
{
	sprite_renderer = new SpriteRenderer();
	initialized = false;
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
	show_scoreboard = false;
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
	else if (button == GLFW_MOUSE_BUTTON_LEFT && Window::mouseCaptured)
	{
		ClientGame::instance()->sendShootPacket();
	}
}

void CPlayState::OnKeyDown(int action, int key)
{
		switch (key)
		{
			// Check if escape was pressed
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(ClientGame::instance()->window, GL_TRUE);
			break;

		case GLFW_KEY_W:
#ifdef _WIN32
			ClientGame::instance()->sendMovePacket(MOVE_FORWARD);
#endif
			break;
		case GLFW_KEY_A:
#ifdef _WIN32
			ClientGame::instance()->sendMovePacket(MOVE_LEFT);
#endif
			break;
		case GLFW_KEY_S:
#ifdef _WIN32
			ClientGame::instance()->sendMovePacket(MOVE_BACKWARD);
#endif
			break;
		case GLFW_KEY_D:
			ClientGame::instance()->sendMovePacket(MOVE_RIGHT);
			break;
		case GLFW_KEY_SPACE: 
			ClientGame::instance()->sendJumpPacket();
			break;
		case GLFW_KEY_TAB:
			if (action == GLFW_REPEAT) {
				show_scoreboard = true;
			}
			break;
		default:
			break;
		}
}
void CPlayState::OnKeyUp(int action, int key) {
	switch (key)
	{
	case GLFW_KEY_TAB:
		show_scoreboard = false;
		break;
	default:
		break;
	}
}

void CPlayState::Update(DWORD dwCurrentTime)
{
		// update scene
		Scene::Instance()->Update();
}

void CPlayState::Draw()  
{ 
	InitTextures();

	Scene::Instance()->Draw();

	// SELF NUMBER OF EGGS
	if (Scene::Instance()->GetPlayer() != NULL) {
		char score[20];
		strcpy_s(score, "Eggs Collected: ");
		strcat_s(score, std::to_string(Scene::Instance()->GetPlayer()->GetScore()).c_str());
		TextRenderer::RenderText(score, Window::width/2 - 100, Window::height - 50, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	}

	if (show_scoreboard) {

		////////////////// BACKGROUND//////////////////////////
		float x = Texture::GetWindowCenter(sb_bg->Width());
		float y = Window::height / 2 - sb_bg->Height() / 2;
		sprite_renderer->DrawSprite(*sb_bg, glm::vec2(x, y), glm::vec2(sb_bg->Width(), sb_bg->Height()), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

		glClear(GL_DEPTH_BUFFER_BIT);

		///////////////// TABLES ////////////////////////////////////////
		x = Texture::GetWindowCenter(sb_table->Width());
		y = Window::height / 2 - sb_table->Height() / 2;
		sprite_renderer->DrawSprite(*sb_table, glm::vec2(x, y), glm::vec2(sb_table->Width(), sb_table->Height()), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

		int our_team = ClientGame::instance()->GetClientTeam();
		printf("our team: %d\n", our_team);
		// us
		TextRenderer::RenderText(std::to_string(ClientGame::instance()->GetScores()[our_team]).c_str(), 25, 115, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));

		// them
		TextRenderer::RenderText(std::to_string(ClientGame::instance()->GetScores()[1 - our_team]).c_str(), 25, 345, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));

		//neutral
		int n = ClientGame::instance()->TotalEggs();
		printf("total eggs: %d\n", n);
		printf("score 0: %d\n", ClientGame::instance()->GetScores()[0]);
		printf("score 1: %d\n", ClientGame::instance()->GetScores()[1]);
		n = n - ClientGame::instance()->GetScores()[0] - ClientGame::instance()->GetScores()[1];
		TextRenderer::RenderText(std::to_string(n).c_str(),25, 595, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
	}

}

void CPlayState::InitTextures() {
	if (!initialized) {
		// Create the different images
		sb_bg = new Texture(GL_TEXTURE_2D, "assets/ui/playstate/scoreboard_bg.png");
		sb_table = new Texture(GL_TEXTURE_2D, "assets/ui/playstate/scoreboard.png");

		initialized = true;
	}
}



