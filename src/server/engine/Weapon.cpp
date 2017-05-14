
#include "Weapon.h"

Weapon::Weapon(btDiscreteDynamicsWorld* curWorld)
    : m_curWorld(curWorld)
{
}

Weapon::Weapon(int firerate, int wdamage, btDiscreteDynamicsWorld* curWorld)
    : m_curWorld(curWorld), m_fireRate(firerate), m_damage(wdamage)
{
}

Weapon::~Weapon()
{
}

// to be overwritten
void Weapon::ReloadWeapon()
{
    m_currentAmmo = m_maxAmmo;
}
