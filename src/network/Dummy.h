#pragma once

#include "GameData.h"

class Dummy
{
public:
    Dummy(int newId, int newx, int newy);

    ~Dummy(void);
    
    void move(int direction);

    // move to specific coordinates
    void move(int newx, int newy);

    int getX();
    int getY();
    int getId();

private:
    int x;
    int y;
    int id;
};