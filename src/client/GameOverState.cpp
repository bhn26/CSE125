#include "GameOverState.h"
#include "LobbyState.h"
#include "StateManager.h"
#include "TextRenderer.h"
#include "Window.h"

#include "../Graphics/Shader.h"
#include "ClientGame.h"
#include "Graphics\Scene.h"

GOState::GOState(CStateManager* pManager) : CGameState(pManager), m_spriteRenderer(new SpriteRenderer())
{
}

GOState::~GOState()
{
    delete m_spriteRenderer;
    delete m_bg;
    delete m_win;
    delete m_lose;
    delete m_tied;
}

GOState* GOState::GetInstance(CStateManager* pManager)
{
    static GOState Instance(pManager);
    return &Instance;
}

void GOState::OnKeyDown(int action, int key)
{
    switch (key)
    {
    case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(ClientGame::instance()->window, GL_TRUE);
        break;
    default:
        break;
    }
}

void GOState::OnClick(int button, int action, double x, double y)
{
    GLubyte res[4];
    GLint viewport[4];

    // render selection
    RenderSelection();

    glGetIntegerv(GL_VIEWPORT, viewport);
    glReadPixels(x, viewport[3] - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &res);

    if (action == GLFW_PRESS)
    {
        switch (res[0])
        {
        case 0:
            printf("None clicked\n");
            break;
        case 1:
            printf("play again clicked\n");
            // todo - reset game
            // change state
            m_stateManager->ChangeState(CMenuState::GetInstance(m_stateManager));
            break;
        default:
            printf("%d clicked%s\n", res[0]);
        }
    }
}

void GOState::RenderSelection()
{
    InitTextures();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ////////////// PLAY AGAIN BUTTON /////////////////////////////////////
}

void GOState::Draw()
{
    InitTextures();

    int x = Texture::GetWindowCenter(m_bg->Width());
    int y = Window::s_height / 2 - m_bg->Height() / 2;

    m_spriteRenderer->DrawSprite(*m_bg,
                                glm::vec2(x, y),
                                glm::vec2(m_bg->Width(), m_bg->Height()),
                                0.0f,
                                glm::vec3(1.0f, 1.0f, 1.0f));

    glClear(GL_DEPTH_BUFFER_BIT); // remove depth info so text and buttons go on top

    //////////////// DISPLAY WINNER ////////////////////////////////
    if (ClientGame::instance()->GetWinner() == -1)
    { // tie
        m_spriteRenderer->DrawSprite(*m_tied,
                                    glm::vec2(x, y),
                                    glm::vec2(m_tied->Width(), m_tied->Height()),
                                    0.0f,
                                    glm::vec3(1.0f, 1.0f, 1.0f));
    }
    else if (ClientGame::instance()->GetWinner() == ClientGame::instance()->GetClientTeam())
    {
        m_spriteRenderer->DrawSprite(*m_win,
                                    glm::vec2(x, y),
                                    glm::vec2(m_win->Width(), m_win->Height()),
                                    0.0f,
                                    glm::vec3(1.0f, 1.0f, 1.0f));
    }
    else
    {
        m_spriteRenderer->DrawSprite(*m_lose,
                                    glm::vec2(x, y),
                                    glm::vec2(m_lose->Width(), m_lose->Height()),
                                    0.0f,
                                    glm::vec3(1.0f, 1.0f, 1.0f));
    }
}

void GOState::EnterState()
{
    ClientGame::instance()->StopMenuSound();
    Window::s_mouseCaptured = false;
    glfwSetInputMode(ClientGame::instance()->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void GOState::Update(DWORD dwCurrentTime)
{
}

void ::GOState::InitTextures()
{
    if (!m_initialized)
    {
        // Create the different images
        m_bg = new Texture(GL_TEXTURE_2D, "assets/ui/gameover/gameover.png");

        m_win = new Texture(GL_TEXTURE_2D, "assets/ui/gameover/m_win.png");
        m_lose = new Texture(GL_TEXTURE_2D, "assets/ui/gameover/m_lose.png");
        m_tied = new Texture(GL_TEXTURE_2D, "assets/ui/gameover/m_tied.png");

        m_initialized = true;
    }
}