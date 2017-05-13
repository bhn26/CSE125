#ifndef _LOADSTATE_H_
#define _LOADSTATE_H_

#include "GameState.h"
#include "MenuState.h"

#include "SpriteRenderer.h"
#include "../Graphics/Texture.h"

class LoadState : public CGameState
{
public:
    ~LoadState();

    void Draw();
    void EnterState();

    void Update(DWORD dwCurrentTime);

    static LoadState* GetInstance(CStateManager* manager);

protected:
    LoadState(CStateManager* manager);

private:
    void InitTextures();

    SpriteRenderer* m_spriteRenderer = nullptr;

    Texture* m_bg = nullptr;
    Texture* m_chickenLeft = nullptr;
    Texture* m_chickenRight = nullptr;
    Texture* m_dot = nullptr;

    bool m_initialized = false;
};

#endif // _LOADSTATE_H_