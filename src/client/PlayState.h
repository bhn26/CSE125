#ifndef _PLAYSTATE_H_
#define _PLAYSTATE_H_

#include "GameState.h"

class CPlayState : public CGameState
{
public:
	~CPlayState();

	// Implementation of specific events
	void OnMouseMove(float xoffset, float yoffset);
	void OnClick(int button, double x, double y);
	void OnKeyDown(WPARAM wKey);
	void Update(DWORD dwCurrentTime);
	void Draw();

	void Reset();
	bool IsGameOver()  { return m_bGameOver; }

	// Returns the single instance
	static CPlayState* GetInstance(CStateManager* pManager);

protected:
	CPlayState(CStateManager* pManager);

private:
	// The text controls to display the current
	// information.
	//CTextControl* m_pScoreControl;

	// The current score
	int scores[2];

	bool m_bGameOver;
};

#endif  // _PLAYSTATE_H_