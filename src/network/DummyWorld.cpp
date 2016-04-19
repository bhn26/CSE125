#include "DummyWorld.h"

DummyWorld::DummyWorld(void)
{
    dummy = nullptr;
    numDummies = 0;
}

DummyWorld::~DummyWorld(void)
{
    delete dummy;
}

bool DummyWorld::canMove(int direction)
{
    return true;
}

void DummyWorld::spawnDummy(int x, int y)
{
    dummy = new Dummy(numDummies, x, y);

}

void DummyWorld::moveDummy(int x, int y)
{
    if(dummy != nullptr)
        dummy->move(x, y);
}

void DummyWorld::moveDummy(int direction)
{
    if (dummy != nullptr)
        dummy->move(direction);
}

PosInfo DummyWorld::getDummyPos()
{
    PosInfo pi;
    if (dummy != nullptr)
    {
        pi.x = dummy->getX();
        pi.y = dummy->getY();
    }
    else
    {
        pi.x = 0;
        pi.y = 0;
    }
    return pi;
}