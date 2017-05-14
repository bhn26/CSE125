#ifndef _LOADSTATE_H_
#define _LOADSTATE_H_

#include "GameState.h"
#include "MenuState.h"

#include "SpriteRenderer.h"
#include "../Graphics/Texture.h"

class LoadState : public CGameState
{
public:
    void Draw();
    void EnterState();

    void Update(DWORD dwCurrentTime);

    static LoadState* GetInstance(CStateManager* manager);

protected:
    LoadState(CStateManager* manager);

private:
    LoadState(const LoadState& rhs) = delete;
    LoadState& operator=(const LoadState& rhs) = delete;
    void InitTextures();

    std::unique_ptr<SpriteRenderer> m_spriteRenderer;

    std::unique_ptr<Texture> m_bg;
    std::unique_ptr<Texture> m_chickenLeft;
    std::unique_ptr<Texture> m_chickenRight;
    std::unique_ptr<Texture> m_dot;

    bool m_initialized = false;
};

#endif // _LOADSTATE_H_