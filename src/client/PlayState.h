#ifndef _PLAYSTATE_H_
#define _PLAYSTATE_H_

#include "GameState.h"

#include "SpriteRenderer.h"
#include "../Graphics/Texture.h"

class CPlayState : public CGameState
{
    friend class ClientGame;
public:
	~CPlayState();

    // Implementation of specific events
    void OnMouseMove(float xoffset, float yoffset) override;
    void OnClick(int button, int action, double x, double y) override;
    void OnKeyDown(int action, int key) override;
    void OnKeyUp(int action, int key) override;
    void Update(DWORD dwCurrentTime) override;
    void Draw() override;

	void Die() { dead = true; };
	void Respawn() { dead = false; };

	void Reset();

	// Returns the single instance
	static CPlayState* GetInstance(CStateManager* pManager);

protected:
	CPlayState(CStateManager* pManager);

private:
	// The current score
	int scores[2];

	bool dead;
	bool show_scoreboard;

	////// HUD ////////////
	bool initialized;
	void InitTextures();

	SpriteRenderer * sprite_renderer;

	Texture * sb_chick;
	Texture * sb_side;
	Texture * sb_table;

	Texture * hud_egg;
	Texture * hud_health;
	Texture * hud_weapon_and_timer;

	Texture * death_overlay;
};

#endif  // _PLAYSTATE_H_