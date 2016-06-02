#include "GameOverState.h"
#include "LobbyState.h"
#include "StateManager.h"
#include "TextRenderer.h"
#include "Window.h"

#include "../Graphics/Shader.h"
#include "ClientGame.h"
#include "Graphics\Scene.h"


GOState::GOState(CStateManager* pManager)
	: CGameState(pManager)
{
	// Create the text controls of the menu.
	sprite_renderer = new SpriteRenderer();
	initialized = false;
}

GOState::~GOState()
{
    delete sprite_renderer;
    delete bg;
}

GOState* GOState::GetInstance(CStateManager* pManager)
{
	static GOState Instance(pManager);
	return &Instance;
}

void GOState::OnKeyDown(WPARAM wKey)
{
}

void GOState::OnClick(int button, double x, double y) {
	GLubyte res[4];
	GLint viewport[4];

	// render selection 
	RenderSelection();

	glGetIntegerv(GL_VIEWPORT, viewport);
	glReadPixels(x, viewport[3] - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &res);

	switch (res[0]) {
	case 0: printf("None clicked\n"); break;
	case 1: printf("play again clicked\n");
		// todo - reset game  
		// change state
		m_pStateManager->ChangeState(CMenuState::GetInstance(m_pStateManager));
		break;
	default: printf("%d clicked%s\n", res[0]);
	}
}

void GOState::RenderSelection() {
	InitTextures();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	////////////// PLAY AGAIN BUTTON /////////////////////////////////////

}


void GOState::Draw()
{
	InitTextures();

	int x = Texture::GetWindowCenter(bg->Width());
	int y = Window::height/2 - bg->Height()/2;

	sprite_renderer->DrawSprite(*bg, glm::vec2(x, y), glm::vec2(bg->Width(), bg->Height()), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	glClear(GL_DEPTH_BUFFER_BIT); // remove depth info so text and buttons go on top

	//////////////// DISPLAY WINNER ////////////////////////////////
	if (ClientGame::instance()->GetWinner() == -1) {
		TextRenderer::RenderText("You Tied!!", x + 275, y + 350, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	}
	else if (ClientGame::instance()->GetWinner() == ClientGame::instance()->GetClientTeam()) {
		TextRenderer::RenderText("You Win!", x + 275, y + 350, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	}
	else {
		TextRenderer::RenderText("You lose :(", x + 275, y + 350, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	}
}

void GOState::EnterState()
{
	Window::mouseCaptured = false;
	glfwSetInputMode(ClientGame::instance()->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void GOState::Update(DWORD dwCurrentTime)
{
}

void::GOState::InitTextures() {
	if (!initialized) {
		// Create the different images
		bg = new Texture(GL_TEXTURE_2D, "assets/ui/gameover/gameover.png");

		initialized = true;
	}
}