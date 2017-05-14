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
    // Deleted because we have unique_ptrs
    LobbyState(const LobbyState& rhs) = delete;
    LobbyState& operator=(const LobbyState& rhs) = delete;

    void RenderSelection();
    void InitTextures();
    void ShowLoadingScreen();

    std::unique_ptr<SpriteRenderer> m_spriteRenderer;

    // stuff below is for optimization later
    std::unique_ptr<Texture> m_bg;
    std::unique_ptr<Texture> m_startButton;

    std::unique_ptr<Texture> m_tableT1;
    std::unique_ptr<Texture> m_tableT2;

    std::unique_ptr<Texture> m_join;
    std::unique_ptr<Texture> m_joinDisabled;

    std::unique_ptr<Texture> m_loadScreen;

    bool m_initialized = false;
    bool m_loading = false;
};

#endif // _LOBBYSTATE_H_