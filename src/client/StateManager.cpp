#include "StateManager.h"
#include "GameState.h"

CStateManager::CStateManager() : m_pActiveState(NULL)
{
}

CStateManager::~CStateManager()
{
}

void CStateManager::OnKeyDown(int action, int key)
{
	if (m_pActiveState)
		m_pActiveState->OnKeyDown(action, key);
}

void CStateManager::OnKeyUp(int action, int key)
{
	if (m_pActiveState)
		m_pActiveState->OnKeyUp(action, key);
}

void CStateManager::OnChar(WPARAM wChar)
{
	if (m_pActiveState)
		m_pActiveState->OnChar(wChar);
}

void CStateManager::Update(DWORD dwCurrentTime)
{
	if (m_pActiveState)
		m_pActiveState->Update(dwCurrentTime);
}

void CStateManager::Draw()
{
	if (m_pActiveState)
		m_pActiveState->Draw();
}