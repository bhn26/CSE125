#ifndef _GAMESTATE_H_
#define _GAMESTATE_H_

#include <WinSock2.h>
#include <Windows.h>

class CStateManager;

// Base class for the different states
// of the game.
class CGameState
{
public:
	// Constructor
	CGameState(CStateManager* pManager);
	// Destructor
	virtual ~CGameState();

	// The different 'events' functions. Child classes can 
	// implement the ones in which they are interested in.
	virtual void OnClick(int button, int action, double x, double y) {}
	virtual void OnMouseMove(float xoffset, float yoffset) {}

	virtual void OnKeyDown(int action, int key ) { }
	virtual void OnKeyUp(int action, int key )   { }
	virtual void OnChar(unsigned int codepoint)   { }
	virtual void Update(DWORD )  { }
	virtual void Draw()  { }

	// Functions called when the state is entered or left
	// (transition from/to another state).
	virtual void EnterState()  { }
	virtual void LeaveState()   { }

protected:
	// Helper function to switch to a new active state.
	void ChangeState(CGameState* pNewState);

	// The state manager.
	CStateManager* m_pStateManager;
};

#endif  // _GAMESTATE_H_