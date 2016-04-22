#include "DummyWorld.h"

DummyWorld::DummyWorld(void)
{
    dummy = new Dummy(0,0,0);
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

void DummyWorld::rotateDummy(float v_rot, float h_rot)
{
    if (dummy != nullptr)
<<<<<<< 174b3f97e4b370bde6db6b896fe55f3188611e9a
        dummy->Rotate(rad);
=======
        dummy->rotate(v_rot, h_rot);
>>>>>>> all rotation done by network and ignore *.db files
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

PosInfo DummyWorld::getDummyRotation() {
    PosInfo pi;
    if (dummy != nullptr)
    {
        pi.v_rotation = dummy->getVRotation();
		pi.h_rotation = dummy->getHRotation();
    }
    else
    {
        pi.v_rotation = 0;
		pi.h_rotation = 0;
    }
    return pi;
}