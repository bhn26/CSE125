#ifndef _LOBBYSTATE_H_
#define _LOBBYSTATE_H_

#include "GameState.h"
#include "PlayState.h"

#include "SpriteRenderer.h"
#include "../Graphics/Texture.h"


// Specialization of the CGameState class for 
// the menu state. This displays a menu in which
// the player can start a new game, continue an 
// existing game, see the high-scores or exit the game.
class LobbyState : public CGameState
{
public:
	~LobbyState();

	void OnKeyDown(WPARAM wKey);
	void OnClick(int button, double x, double y);
	void Draw();
	void EnterState();

	static LobbyState* GetInstance(CStateManager* pManager);

protected:
	LobbyState(CStateManager* pManager);

private:
	void RenderSelection();
	void InitTextures();

	SpriteRenderer * sprite_renderer;

	// stuff below is for optimization later
	Texture *logo;
	Texture *panel;
	Texture *start_button;

	bool initialized;
};

#endif  // _LOBBYSTATE_H_