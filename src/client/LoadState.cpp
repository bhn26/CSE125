#include "LoadState.h"

#include "StateManager.h"
#include "TextRenderer.h"
#include "Window.h"

#include "Graphics/Shader.h"
#include "ClientGame.h"
#include "Graphics\Scene.h"

LoadState::LoadState(CStateManager* manager)
    : CGameState(manager), m_spriteRenderer(std::make_unique<SpriteRenderer>())
{
}

LoadState* LoadState::GetInstance(CStateManager* manager)
{
    static LoadState Instance(manager);
    return &Instance;
}

void LoadState::Draw()
{
    InitTextures();

    int x = Texture::GetWindowCenter(m_bg->Width());
    int y = Window::s_height / 2 - m_bg->Height() / 2;

    m_spriteRenderer->DrawSprite(m_bg.get(),
                                 glm::vec2(x, y),
                                 glm::vec2(m_bg->Width(), m_bg->Height()),
                                 0.0f,
                                 glm::vec3(1.0f, 1.0f, 1.0f));
}

void LoadState::EnterState()
{
}

void LoadState::Update(DWORD dwCurrentTime)
{
}

void ::LoadState::InitTextures()
{
    if (!m_initialized)
    {
        // Create the different images
        m_bg = std::make_unique<Texture>(GL_TEXTURE_2D, "assets/ui/loading/loading_bg.png");

        m_chickenLeft =
            std::make_unique<Texture>(GL_TEXTURE_2D, "assets/ui/loading/loading_chicken_l.png");
        m_chickenRight =
            std::make_unique<Texture>(GL_TEXTURE_2D, "assets/ui/loading/loading_chicken_r.png");
        m_dot = std::make_unique<Texture>(GL_TEXTURE_2D, "assets/ui/loading/loading_m_dot.png");

        m_initialized = true;
    }
}