#include "StateManager.h"
#include "GameState.h"

void CStateManager::OnKeyDown(int action, int key)
{
    if (m_activeState)
    {
        m_activeState->OnKeyDown(action, key);
    }
}

void CStateManager::OnKeyUp(int action, int key)
{
    if (m_activeState)
    {
        m_activeState->OnKeyUp(action, key);
    }
}

void CStateManager::OnChar(unsigned int keycode)
{
    if (m_activeState)
    {
        m_activeState->OnChar(keycode);
    }
}

void CStateManager::Update(DWORD dwCurrentTime)
{
    if (m_activeState)
    {
        m_activeState->Update(dwCurrentTime);
    }
}

void CStateManager::Draw()
{
    if (m_activeState)
    {
        m_activeState->Draw();
    }
}