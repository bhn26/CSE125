#ifndef _PLAYSTATE_H_
#define _PLAYSTATE_H_

#include "GameState.h"

#include "SpriteRenderer.h"
#include "Graphics/Texture.h"

class CPlayState : public CGameState
{
    friend class ClientGame;

public:
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
    CPlayState(const CPlayState& rhs) = delete;
    CPlayState& operator=(const CPlayState& rhs) = delete;

    // The current score
    int m_scores[2] = {0,0};

    bool m_dead = false;
    bool m_showScoreboard = false;

    int m_backgroundMusicID = -1;

    ////// HUD ////////////
    bool m_initialized = false;
    void InitTextures();

    std::unique_ptr<SpriteRenderer> m_spriteRenderer;

    std::unique_ptr<Texture> m_sbChick;
    std::unique_ptr<Texture> m_sbSide;
    std::unique_ptr<Texture> m_sbTable;

    std::unique_ptr<Texture> m_hudEgg;
    std::unique_ptr<Texture> m_hudHealth;
    std::unique_ptr<Texture> m_hudPower;
    std::unique_ptr<Texture> m_hudWeaponAndTimer;

    std::unique_ptr<Texture> m_hudTomato;
    std::unique_ptr<Texture> m_hudPotato;
    std::unique_ptr<Texture> m_hudPumpkinSeed;
    std::unique_ptr<Texture> m_hudBB;
    std::unique_ptr<Texture> m_hudMine;
    std::unique_ptr<Texture> m_hudSeeds;

    std::unique_ptr<Texture> m_weaponMissing;

    Texture* m_hudWeapon = nullptr;

    std::unique_ptr<Texture> m_deathOverlay;
};

#endif // _PLAYSTATE_H_