#ifndef _MENUSTATE_H_
#define _MENUSTATE_H_

#include "GameState.h"
#include "PlayState.h"

#include "SpriteRenderer.h"
#include "Graphics/Texture.h"

// Specialization of the CGameState class for
// the menu state. This displays a menu in which
// the player can start a new game, continue an
class CMenuState : public CGameState
{
public:
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
    // Deleted because we have unique_ptrs
    CMenuState(const CMenuState& rhs) = delete;
    CMenuState& operator=(const CMenuState& rhs) = delete;

    void StartGame();
    void RenderSelection();
    void InitTextures();

    std::unique_ptr<SpriteRenderer> m_spriteRenderer;

    // stuff below is for optimization later
    std::unique_ptr<Texture> m_bg;
    std::unique_ptr<Texture> m_textbox;
    std::unique_ptr<Texture> m_join;

    std::string m_defaultName;
    std::string m_username;

    bool m_typing = false; // true when player selects textbox

    bool m_initialized = false;
};

#endif // _MENUSTATE_H_