#pragma once
#include "Dummy.h"
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


    private:
        std::vector<Dummy *> dummies;
        int numDummies;
};