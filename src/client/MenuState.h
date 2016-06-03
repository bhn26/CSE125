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

	void OnKeyDown(int action, int key) override;
	void OnClick(int button, int action, double x, double y) override;
	void OnChar(unsigned int codepoint) override;

	void Update(DWORD) override;
	void Draw() override;
	void EnterState() override;

	static CMenuState* GetInstance(CStateManager* pManager);

protected:
	CMenuState(CStateManager* pManager);

private:
	void StartGame();
	void RenderSelection();
	void InitTextures();

	SpriteRenderer * sprite_renderer;

	// stuff below is for optimization later
	Texture *bg;
	Texture *textbox;
	Texture *join;

	std::string default_name;
	std::string username;

	bool typing; // true when player selects textbox
	
	bool initialized;
};

#endif  // _MENUSTATE_H_