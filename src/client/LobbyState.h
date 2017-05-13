#ifndef _LOBBYSTATE_H_
#define _LOBBYSTATE_H_

#include "GameState.h"
#include "PlayState.h"

#include "SpriteRenderer.h"
#include "Graphics/Texture.h"

// Specialization of the CGameState class for
// the menu state. This displays a menu in which
// the player can start a new game, continue an
// existing game, see the high-scores or exit the game.
class LobbyState : public CGameState
{
public:
    ~LobbyState();

    void OnKeyDown(WPARAM wKey);
    void OnClick(int button, int action, double x, double y) override;
    void Draw() override;
    void EnterState() override;
    void LeaveState() override;

    static LobbyState* GetInstance(CStateManager* pManager);

    void ServerLoading() { m_loading = true; }

protected:
    LobbyState(CStateManager* pManager);

private:
    void RenderSelection();
    void InitTextures();
    void ShowLoadingScreen();

    SpriteRenderer* m_spriteRenderer;

    // stuff below is for optimization later
    Texture* m_bg = nullptr;
    Texture* m_startButton = nullptr;

    Texture* m_tableT1 = nullptr;
    Texture* m_tableT2 = nullptr;

    Texture* m_join = nullptr;
    Texture* m_joinDisabled = nullptr;

    Texture* m_loadScreen = nullptr;

    bool m_initialized = false;
    bool m_loading = false;
};

#endif // _LOBBYSTATE_H_