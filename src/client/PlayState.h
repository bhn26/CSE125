#ifndef _PLAYSTATE_H_
#define _PLAYSTATE_H_

#include "GameState.h"
#include "GameFont.h"
#include "TextControl.h"
#include "Image.h"

class CPlayState : public CGameState
{
public:
	~CPlayState();

	// Implementation of specific events
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
	CTextControl* m_pScoreControl;

	// The font used to draw text
	CGameFont* m_pFont;

	// The current score
	ULONG m_ulCurrentScore;

	bool m_bGameOver;

	// The background image
	TImagePtr m_pBackgroundImg;
};

#endif  // _PLAYSTATE_H_