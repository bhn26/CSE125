#include "DummyWorld.h"

DummyWorld::DummyWorld(void)
{
    numDummies = 0;
}

DummyWorld::~DummyWorld(void)
{

}

bool DummyWorld::canMove(int direction)
{
    return true;
}

void DummyWorld::spawnDummy(int x, int y)
{
    Dummy *d = new Dummy(numDummies, x, y);
    numDummies++;
    dummies.push_back(d);
}