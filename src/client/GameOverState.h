#ifndef _GAMEOVERSTATE_H_
#define _GAMEOVERSTATE_H_

#include "GameState.h"
#include "MenuState.h"

#include "SpriteRenderer.h"
#include "Graphics/Texture.h"

// Specialization of the CGameState class for
// the menu state. This displays a menu in which
// the player can start a new game, continue an
// existing game, see the high-scores or exit the game.
class GOState : public CGameState
{
public:
    void OnKeyDown(int action, int key) override;
    void OnClick(int button, int action, double x, double y) override;
    void Draw() override;
    void EnterState() override;

    void Update(DWORD dwCurrentTime) override;

    static GOState* GetInstance(CStateManager* pManager);

protected:
    GOState(CStateManager* pManager);

private:
    GOState(const GOState& rhs) = delete;
    GOState& operator=(const GOState& rhs) = delete;

    void RenderSelection();
    void InitTextures();

    std::unique_ptr<SpriteRenderer> m_spriteRenderer;

    std::unique_ptr<Texture> m_bg;
    std::unique_ptr<Texture> m_win;
    std::unique_ptr<Texture> m_lose;
    std::unique_ptr<Texture> m_tied;

    bool m_initialized = false;
};

#endif // _GAMEOVERSTATE_H_