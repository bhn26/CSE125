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
	void OnClick(int button, int action, double x, double y) override;
	void Draw() override;
	void EnterState() override;

	static LobbyState* GetInstance(CStateManager* pManager);

protected:
	LobbyState(CStateManager* pManager);

private:
	void RenderSelection();
	void InitTextures();
	void ShowLoadingScreen();

	SpriteRenderer * sprite_renderer;

	// stuff below is for optimization later
	Texture *bg;
	Texture *start_button;

	Texture *table_t1;
	Texture *table_t2;

	Texture *join;
	Texture *join_disabled;

	Texture * load_screen;

	bool initialized;
	bool loading;
};

#endif  // _LOBBYSTATE_H_