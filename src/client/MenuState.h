#ifndef _MENUSTATE_H_
#define _MENUSTATE_H_

#include "GameState.h"
#include "PlayState.h"

#include "SpriteRenderer.h"
#include "../Graphics/Texture.h"


// Specialization of the CGameState class for 
// the menu state. This displays a menu in which
// the player can start a new game, continue an 
// existing game, see the high-scores or exit the game.
class CMenuState : public CGameState
{
public:
	~CMenuState();

	void OnKeyDown(WPARAM wKey);
	void OnClick(double x, double y);
	void Draw();
	void EnterState();

	static CMenuState* GetInstance(CStateManager* pManager);

protected:
	CMenuState(CStateManager* pManager);

private:
	// The player went up or down in 
	// the menu
	void SelectionUp();
	void SelectionDown();
	// The player validated the current selection
	void SelectionChosen();

	void RenderSelection();

	// Index of the current selected menu item
	int m_iCurrentSelection;

	// A pointer to the current active game (if any).
	CPlayState* m_pCurrentGame;

	SpriteRenderer * sprite_renderer;

	// stuff below is for optimization later
	Texture *logo;
	Texture *textbox;
	Texture *join;
};

#endif  // _MENUSTATE_H_