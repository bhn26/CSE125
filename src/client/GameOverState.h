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
    ~GOState();

    void OnKeyDown(int action, int key) override;
    void OnClick(int button, int action, double x, double y) override;
    void Draw() override;
    void EnterState() override;

    void Update(DWORD dwCurrentTime);

    static GOState* GetInstance(CStateManager* pManager);

protected:
    GOState(CStateManager* pManager);

private:
    void RenderSelection();
    void InitTextures();

    SpriteRenderer* m_spriteRenderer = nullptr;

    Texture* m_bg = nullptr;
    Texture* m_win = nullptr;
    Texture* m_lose = nullptr;
    Texture* m_tied = nullptr;

    bool m_initialized = false;
};

#endif // _GAMEOVERSTATE_H_