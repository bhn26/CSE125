#ifndef _STATEMANAGER_H_
#define _STATEMANAGER_H_

#include "GameState.h"

// Manages the different states of the game.
class CStateManager
{
public:
    // Default constructor
    CStateManager() = default;
    // Default destructor
    ~CStateManager() = default;

    // Switches to another active state.
    void ChangeState(CGameState* pNewState)
    {
        if (m_activeState)
        {
            m_activeState->LeaveState();
        }
        m_activeState = pNewState;
        m_activeState->EnterState();
    }

    // Returns the current active state.
    CGameState* GetActiveState() { return m_activeState; }

    // 'Events' function, they are simply redirected to
    // the active state.
    void OnKeyDown(int action, int key);
    void OnKeyUp(int action, int key);
    void OnChar(unsigned int keycode);
    void Update(DWORD dwCurrentTime);
    void Draw();

private:
    // Active State of the game (intro, play, ...)
    CGameState* m_activeState = nullptr;
};

#endif // _STATEMANAGER_H_