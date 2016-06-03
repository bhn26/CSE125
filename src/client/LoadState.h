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

	static LoadState* GetInstance(CStateManager* pManager);

protected:
	LoadState(CStateManager* pManager);

private:
	void InitTextures();

	SpriteRenderer * sprite_renderer;

	Texture * bg;
	Texture * chicken_left;
	Texture * chicken_right;
	Texture * dot;

	bool initialized;
};

#endif  // _LOADSTATE_H_