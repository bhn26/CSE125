#ifndef _MENUSTATE_H_
#define _MENUSTATE_H_

#include "GameState.h"
#include "PlayState.h"

#include "SpriteRenderer.h"
#include "Graphics/Texture.h"

// Specialization of the CGameState class for
// the menu state. This displays a menu in which
// the player can start a new game, continue an
// existing game, see the high-scores or exit the game.
class CMenuState : public CGameState
{
public:
    ~CMenuState();

    void OnKeyDown(int action, int key) override;
    void OnClick(int button, int action, double x, double y) override;
    void OnChar(unsigned int codepoint) override;

    void Update(DWORD) override;
    void Draw() override;
    void EnterState() override;

    static CMenuState* GetInstance(CStateManager* manager);

protected:
    CMenuState(CStateManager* manager);

private:
    void StartGame();
    void RenderSelection();
    void InitTextures();

    SpriteRenderer* m_spriteRenderer = nullptr;

    // stuff below is for optimization later
    Texture* m_bg = nullptr;
    Texture* m_textbox = nullptr;
    Texture* m_join = nullptr;

    std::string m_defaultName;
    std::string m_username;

    bool m_typing = false; // true when player selects textbox

    bool m_initialized = false;
};

#endif // _MENUSTATE_H_