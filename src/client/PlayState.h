#ifndef _PLAYSTATE_H_
#define _PLAYSTATE_H_

#include "GameState.h"

#include "SpriteRenderer.h"
#include "Graphics/Texture.h"

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

    void Die() { m_dead = true; };
    void Respawn() { m_dead = false; };

    void EnterState() override;
    void LeaveState() override;
    void Reset();

    // Returns the single instance
    static CPlayState* GetInstance(CStateManager* pManager);

protected:
    CPlayState(CStateManager* pManager);

private:
    // The current score
    int m_scores[2] = [0,0];

    bool m_dead = false;
    bool m_showScoreboard = false;

    int m_backgroundMusicID = -1;

    ////// HUD ////////////
    bool m_initialized = false;
    void InitTextures();

    SpriteRenderer* m_spriteRenderer = nullptr;

    Texture* m_sbChick = nullptr;
    Texture* m_sbSide = nullptr;
    Texture* m_sbTable = nullptr;

    Texture* m_hudEgg = nullptr;
    Texture* m_hudHealth = nullptr;
    Texture* m_hudPower = nullptr;
    Texture* m_hudWeaponAndTimer = nullptr;

    Texture* m_hudTomato = nullptr;
    Texture* m_hudPotato = nullptr;
    Texture* m_hudPumpkinSeed = nullptr;
    Texture* m_hudBB = nullptr;
    Texture* m_hudMine = nullptr;
    Texture* m_hudSeeds = nullptr;

    Texture* m_weaponMissing = nullptr;

    Texture* m_hudWeapon = nullptr;

    Texture* m_deathOverlay = nullptr;
};

#endif // _PLAYSTATE_H_