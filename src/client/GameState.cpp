#include "GameState.h"
#include "StateManager.h"

CGameState::CGameState(CStateManager* manager) : m_stateManager(manager)
{
}

CGameState::~CGameState()
{
}

void CGameState::ChangeState(CGameState* newState)
{
    m_stateManager->ChangeState(newState);
}