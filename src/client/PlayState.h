#ifndef _PLAYSTATE_H_
#define _PLAYSTATE_H_

#include "GameState.h"

#include "SpriteRenderer.h"
#include "../Graphics/Texture.h"

class CPlayState : public CGameState
{
public:
	~CPlayState();

	// Implementation of specific events
	void OnMouseMove(float xoffset, float yoffset);
	void OnClick(int button, double x, double y);
	void OnKeyDown(int action, int key);
	void OnKeyUp(int action, int key);
	void Update(DWORD dwCurrentTime);
	void Draw();

	void Reset();

	// Returns the single instance
	static CPlayState* GetInstance(CStateManager* pManager);

protected:
	CPlayState(CStateManager* pManager);

private:
	// The current score
	int scores[2];

	bool show_scoreboard;

	////// HUD ////////////
	bool initialized;
	void InitTextures();

	SpriteRenderer * sprite_renderer;

	Texture* sb_bg;
	Texture* sb_table;
};

#endif  // _PLAYSTATE_H_