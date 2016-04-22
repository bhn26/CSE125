#pragma once
#include "Dummy.h"
#include "GameData.h"
#include <vector>

#define MAXPOS 5
#define MAXNEG -5

class DummyWorld
{
    public:
        DummyWorld(void);
        ~DummyWorld(void);
        bool canMove(int direction);
        void spawnDummy(int x, int y);

        // move dummy in a direction
        void moveDummy(int direction);

        // move dummy to specific position
        void moveDummy(int x, int y);

        void rotateDummy(float rad);

        PosInfo getDummyPos();

        PosInfo getDummyRotation();

    private:
        Dummy* dummy;
        int numDummies;
};