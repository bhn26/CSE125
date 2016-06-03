#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "PlayState.h"
#include "StateManager.h"
#include "MenuState.h"
#include <sstream>
#include <math.h>

#include "Graphics/Scene.h"
#include "ConfigManager.h"
#include "Player.h"
#include "client/ClientGame.h"
#include "client/TextRenderer.h"

using namespace std;

CPlayState::CPlayState(CStateManager* pManager)
 : CGameState(pManager), show_scoreboard(false)
{
	sprite_renderer = new SpriteRenderer();
	initialized = false;
	dead = false;
}

CPlayState::~CPlayState()
{
	delete sprite_renderer;

	delete sb_chick;
	delete sb_side;
	delete sb_table;

	delete hud_egg;
	delete hud_health;
	delete hud_weapon_and_timer;
	delete hud_tomato;
	delete hud_potato;
	delete hud_pumpkin_seed;

	delete death_overlay;
}

CPlayState* CPlayState::GetInstance(CStateManager* pManager)
{
	static CPlayState Instance(pManager);
	return &Instance;
}

void CPlayState::Reset()
{
	dead = false;
	show_scoreboard = false;
}

void CPlayState::OnMouseMove(float xoffset, float yoffset) {
	Player * player = Scene::Instance()->GetPlayer();
	if (player != NULL) {
		player->ProcessMouseMovement(xoffset, yoffset);
	}
}

void CPlayState::OnClick(int button, int action, double x, double y) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && !Window::mouseCaptured)
	{
		Window::mouseCaptured = true;
		Window::firstMouse = true;
		glfwSetInputMode(ClientGame::instance()->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE && Window::mouseCaptured)
	{
		Window::mouseCaptured = false;
		glfwSetInputMode(ClientGame::instance()->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && Window::mouseCaptured)
	{
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
			ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_Right_Click"));
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && Window::mouseCaptured)
	{
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
			ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_Left_Click"));
	}
}

void CPlayState::OnKeyDown(int action, int key)
{
        switch (key)
        {
        case GLFW_KEY_ESCAPE:
            ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_Esc"));
            break;
        case GLFW_KEY_A:
            ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_A"));
            break;
        case GLFW_KEY_B:
            ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_B"));
            break;
        case GLFW_KEY_C:
            ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_C"));
            break;
        case GLFW_KEY_D:
            ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_D"));
            break;
        case GLFW_KEY_E:
            ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_E"));
            break;
        case GLFW_KEY_F:
            ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_F"));
            break;
        case GLFW_KEY_G:
            ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_G"));
            break;
        case GLFW_KEY_H:
            ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_H"));
            break;
        case GLFW_KEY_I:
            ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_I"));
            break;
        case GLFW_KEY_J:
            ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_J"));
            break;
        case GLFW_KEY_K:
            ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_K"));
            break;
        case GLFW_KEY_L:
            ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_L"));
            break;
        case GLFW_KEY_M:
            ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_M"));
            break;
        case GLFW_KEY_N:
            ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_N"));
            break;
        case GLFW_KEY_O:
            ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_O"));
            break;
        case GLFW_KEY_P:
            ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_P"));
            break;
        case GLFW_KEY_Q:
            ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_Q"));
            break;
        case GLFW_KEY_R:
            ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_R"));
            break;
        case GLFW_KEY_S:
            ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_S"));
            break;
        case GLFW_KEY_T:
            ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_T"));
            break;
        case GLFW_KEY_U:
            ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_U"));
            break;
        case GLFW_KEY_V:
            ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_V"));
            break;
        case GLFW_KEY_W:
            ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_W"));
            break;
        case GLFW_KEY_X:
            ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_X"));
            break;
        case GLFW_KEY_Y:
            ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_Y"));
            break;
        case GLFW_KEY_Z:
            ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_Z"));
            break;
        case GLFW_KEY_0:
            ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_0"));
            break;
        case GLFW_KEY_1:
            ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_1"));
            break;
        case GLFW_KEY_2:
            ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_2"));
            break;
        case GLFW_KEY_3:
            ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_3"));
            break;
        case GLFW_KEY_4:
            ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_4"));
            break;
        case GLFW_KEY_5:
            ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_5"));
            break;
        case GLFW_KEY_6:
            ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_6"));
            break;
        case GLFW_KEY_7:
            ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_7"));
            break;
        case GLFW_KEY_8:
            ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_8"));
            break;
        case GLFW_KEY_9:
            ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_9"));
            break;
        case GLFW_KEY_SPACE: 
            ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_Space"));
            break;
        case GLFW_KEY_TAB:
            // Check if escape was pressed
            ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_Tab"));
            break;
        default:
            break;
        }
}

void CPlayState::OnKeyUp(int action, int key)
{
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_Esc"), false);
		break;
	case GLFW_KEY_A:
		ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_A"), false);
		break;
	case GLFW_KEY_B:
		ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_B"), false);
		break;
	case GLFW_KEY_C:
		ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_C"), false);
		break;
	case GLFW_KEY_D:
		ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_D"), false);
		break;
	case GLFW_KEY_E:
		ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_E"), false);
		break;
	case GLFW_KEY_F:
		ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_F"), false);
		break;
	case GLFW_KEY_G:
		ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_G"), false);
		break;
	case GLFW_KEY_H:
		ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_H"), false);
		break;
	case GLFW_KEY_I:
		ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_I"), false);
		break;
	case GLFW_KEY_J:
		ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_J"), false);
		break;
	case GLFW_KEY_K:
		ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_K"), false);
		break;
	case GLFW_KEY_L:
		ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_L"), false);
		break;
	case GLFW_KEY_M:
		ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_M"), false);
		break;
	case GLFW_KEY_N:
		ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_N"), false);
		break;
	case GLFW_KEY_O:
		ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_O"), false);
		break;
	case GLFW_KEY_P:
		ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_P"), false);
		break;
	case GLFW_KEY_Q:
		ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_Q"), false);
		break;
	case GLFW_KEY_R:
		ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_R"), false);
		break;
	case GLFW_KEY_S:
		ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_S"), false);
		break;
	case GLFW_KEY_T:
		ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_T"), false);
		break;
	case GLFW_KEY_U:
		ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_U"), false);
		break;
	case GLFW_KEY_V:
		ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_V"), false);
		break;
	case GLFW_KEY_W:
		ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_W"), false);
		break;
	case GLFW_KEY_X:
		ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_X"), false);
		break;
	case GLFW_KEY_Y:
		ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_Y"), false);
		break;
	case GLFW_KEY_Z:
		ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_Z"), false);
		break;
	case GLFW_KEY_0:
		ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_0"), false);
		break;
	case GLFW_KEY_1:
		ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_1"), false);
		break;
	case GLFW_KEY_2:
		ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_2"), false);
		break;
	case GLFW_KEY_3:
		ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_3"), false);
		break;
	case GLFW_KEY_4:
		ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_4"), false);
		break;
	case GLFW_KEY_5:
		ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_5"), false);
		break;
	case GLFW_KEY_6:
		ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_6"), false);
		break;
	case GLFW_KEY_7:
		ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_7"), false);
		break;
	case GLFW_KEY_8:
		ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_8"), false);
		break;
	case GLFW_KEY_9:
		ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_9"), false);
		break;
	case GLFW_KEY_SPACE:
		ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_Space"), false);
		break;
	case GLFW_KEY_TAB:
		// Check if escape was pressed
		ClientGame::instance()->HandleButtonEvent(ConfigManager::instance()->GetConfigValue("PC_Tab"), false);
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

	//////////////// SCOREBOARD ///////////////////////////////////
	if (show_scoreboard) {
		int our_team = ClientGame::instance()->GetClientTeam();

		////////////////// BACKGROUND//////////////////////////
		int x = Window::width - sb_chick->Width();
		int y = Window::height - sb_chick->Height();
		sprite_renderer->DrawSprite(*sb_chick, glm::vec2(x, y), glm::vec2(sb_chick->Width(), sb_chick->Height()), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

		int side_x = 0;
		int side_y = Window::height / 2 - sb_side->Height() / 2;
		sprite_renderer->DrawSprite(*sb_side, glm::vec2(side_x, side_y), glm::vec2(sb_side->Width(), sb_side->Height()), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

		glClear(GL_DEPTH_BUFFER_BIT);

		///////////////// TABLES ////////////////////////////////////////
		x = Texture::GetWindowCenter(sb_table->Width());
		y = Window::height / 2 - sb_table->Height() / 2;
		sprite_renderer->DrawSprite(*sb_table, glm::vec2(x, y), glm::vec2(sb_table->Width(), sb_table->Height()), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

		glClear(GL_DEPTH_BUFFER_BIT);

		// team 0
		std::vector<int> team0 = ClientGame::Team0();

		int y0 = y + 140, y1 = y0;
		int x0, x1;
		if (our_team == 0) {
			x0 = x + 445; // our team displays on right
			x1 = x + 45;
		}
		else {
			x0 = x + 45;
			x1 = x + 445;
		}

		for (int i = 0; i < team0.size(); i++) {
			char score[20];
			strcpy_s(score, "Player ");
			strcat_s(score, std::to_string(team0.at(i)).c_str());
			strcat_s(score, ": ");
			Player * player = (Player *) Scene::Instance()->GetEntity(PLAYER, team0.at(i)).get();
			strcat_s(score, std::to_string(player->GetScore()).c_str());
			TextRenderer::RenderText(score, x0, y0, 1.0f, glm::vec3(0.5f, 0.5f, 0.5f));

			y0 = y0 + 80; // row height
		}

		// team 1
		std::vector<int> team1 = ClientGame::Team1();
		for (int i = 0; i < team1.size(); i++) {
			char score[20];
			strcpy_s(score, "Player ");
			strcat_s(score, std::to_string(team1.at(i)).c_str());
			strcat_s(score, ": ");
			Player * player = (Player *)Scene::Instance()->GetEntity(PLAYER, team1.at(i)).get();
			strcat_s(score, std::to_string(player->GetScore()).c_str());
			TextRenderer::RenderText(score, x1, y1, 1.0f, glm::vec3(0.5f, 0.5f, 0.5f));

			y1 = y1 + 80; // row height
		}

		//////////////// TEAM SCORES ///////////////////////////
		printf("our team: %d\n", our_team);
		// us
		TextRenderer::RenderText(std::to_string(ClientGame::instance()->GetScores()[our_team]).c_str(), 25, side_y + 325, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));

		// them
		TextRenderer::RenderText(std::to_string(ClientGame::instance()->GetScores()[1 - our_team]).c_str(), 25, side_y + 560, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));

		//neutral
		int n = ClientGame::instance()->TotalEggs();
		printf("total eggs: %d\n", n);
		printf("score 0: %d\n", ClientGame::instance()->GetScores()[0]);
		printf("score 1: %d\n", ClientGame::instance()->GetScores()[1]);
		n = n - ClientGame::instance()->GetScores()[0] - ClientGame::instance()->GetScores()[1];
		TextRenderer::RenderText(std::to_string(n).c_str(),25, side_y + 805, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));

	} else if (Scene::Instance()->GetPlayer() != NULL) {
		int x = 20;
		int y = Window::height - hud_egg->Height() - 20;

		//////////////// EGGS COLLECTED ///////////////////////////
		int score = Scene::Instance()->GetPlayer()->GetScore();
		for (int i = 0; i < score; i++) {
			sprite_renderer->DrawSprite(*hud_egg, glm::vec2(x, y), glm::vec2(hud_egg->Width(), hud_egg->Height()), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
			x = x + hud_egg->Width() + 5;
		}

		////////////// WEAPON & TIMER ///////////////////////////////////
		x = Window::width - hud_weapon_and_timer->Width() - 20;
		y = Window::height - hud_weapon_and_timer->Height() - 20;
		sprite_renderer->DrawSprite(*hud_weapon_and_timer, glm::vec2(x, y), glm::vec2(hud_weapon_and_timer->Width(), hud_weapon_and_timer->Height()), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

		switch (Scene::Instance()->GetPlayer()->GetWeapon()) {
		case SEEDGUN:
			hud_weapon = hud_pumpkin_seed;
			break;
		case BOUNCEGUN:
			hud_weapon = hud_tomato;
			break;
		case GRENADELAUNCHER:
			hud_weapon = hud_potato;
			break;
		default:
			break;
		}

		glClear(GL_DEPTH_BUFFER_BIT);
		if (Scene::Instance()->GetPlayer()->GetWeapon() != -1) {
			sprite_renderer->DrawSprite(*hud_weapon, glm::vec2(x, y), glm::vec2(hud_weapon->Width(), hud_weapon->Height()), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
		}

		int timer = ClientGame::instance()->GetCountdown();
		int minutes = timer / 60;
		int seconds = timer % 60;
		std::string time;
		if (timer == 0)
		{
			time += "5";
			time += ":";
			time += "00";
		}
		else
		{
			time += std::to_string(minutes);
			time += ":";
			if (seconds == 0)
				time += "00";
			else if(seconds < 10)
			{
				time += "0";
				time += std::to_string(seconds);
			}
			else
				time += std::to_string(seconds);
		}
		glClear(GL_DEPTH_BUFFER_BIT);
		TextRenderer::RenderText(time.c_str(), x+54, y+190, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));

		////////////////////// HEALTH BAR /////////////////////////////////
		x = Texture::GetWindowCenter(hud_health->Width());
		y = 20;

		float health = Scene::Instance()->GetPlayer()->GetHealth();
		std::shared_ptr<Shader>& shader = sprite_renderer->GetShader();
		GLint health_pos = shader->GetUniform("health_x_pos");
		glUniform1i(health_pos, x + (hud_health->Width()*(health/100.0f)));
		sprite_renderer->DrawSprite(*hud_health, glm::vec2(x, y), glm::vec2(hud_health->Width(), hud_health->Height()), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniform1i(health_pos, 0);

		/////////// DEATH OVERLAY ////////////////////////////////////
		if (dead) { // play loud sound
			glClear(GL_DEPTH_BUFFER_BIT);

			x = Texture::GetWindowCenter(death_overlay->Width());
			y = Window::height / 2 - death_overlay->Height() / 2;

			sprite_renderer->DrawSprite(*death_overlay, glm::vec2(x, y), glm::vec2(death_overlay->Width(), death_overlay->Height()), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
		}
	}
}

void CPlayState::InitTextures() {
	if (!initialized) {
		// Create the different images
		sb_chick = new Texture(GL_TEXTURE_2D, "assets/ui/playstate/scoreboard_chicken.png");
		sb_side = new Texture(GL_TEXTURE_2D, "assets/ui/playstate/sb_side_panel.png");
		sb_table = new Texture(GL_TEXTURE_2D, "assets/ui/playstate/scoreboard.png");

		hud_egg = new Texture(GL_TEXTURE_2D, "assets/ui/playstate/hud_egg.png");
		hud_health = new Texture(GL_TEXTURE_2D, "assets/ui/playstate/hud_health.png");
		hud_weapon_and_timer = new Texture(GL_TEXTURE_2D, "assets/ui/playstate/hud_weapon&timer.png");
		hud_tomato = new Texture(GL_TEXTURE_2D, "assets/ui/playstate/tomato_icon.png");
		hud_potato = new Texture(GL_TEXTURE_2D, "assets/ui/playstate/potato_icon.png");
		hud_pumpkin_seed = new Texture(GL_TEXTURE_2D, "assets/ui/playstate/pumpkin_seed_icon.png");

		death_overlay = new Texture(GL_TEXTURE_2D, "assets/ui/playstate/death.png");

		initialized = true;
	}
}



