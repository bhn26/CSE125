#include "LoadState.h"
#include "StateManager.h"
#include "TextRenderer.h"
#include "Window.h"

#include "../Graphics/Shader.h"
#include "ClientGame.h"
#include "Graphics\Scene.h"


LoadState::LoadState(CStateManager* pManager)
	: CGameState(pManager)
{
	// Create the text controls of the menu.
	sprite_renderer = new SpriteRenderer();
	initialized = false;
}

LoadState::~LoadState()
{
	delete sprite_renderer;
	delete bg;
	delete chicken_left;
	delete chicken_right;
	delete dot;
}

LoadState* LoadState::GetInstance(CStateManager* pManager)
{
	static LoadState Instance(pManager);
	return &Instance;
}


void LoadState::Draw()
{
	InitTextures();

	int x = Texture::GetWindowCenter(bg->Width());
	int y = Window::height / 2 - bg->Height() / 2;

	sprite_renderer->DrawSprite(*bg, glm::vec2(x, y), glm::vec2(bg->Width(), bg->Height()), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
}

void LoadState::EnterState()
{
}

void LoadState::Update(DWORD dwCurrentTime)
{
}

void::LoadState::InitTextures() {
	if (!initialized) {
		// Create the different images
		bg = new Texture(GL_TEXTURE_2D, "assets/ui/loading/loading_bg.png");

		chicken_left = new Texture(GL_TEXTURE_2D, "assets/ui/loading/loading_chicken_l.png");
		chicken_right = new Texture(GL_TEXTURE_2D, "assets/ui/loading/loading_chicken_r.png");
		dot = new Texture(GL_TEXTURE_2D, "assets/ui/loading/loading_dot.png");

		initialized = true;
	}
}