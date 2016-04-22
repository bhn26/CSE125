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

    void Rotate(float rad);

    int getX();
    int getY();
    float getRotation() {return radians; };
    int getId();

private:
    int x;
    int y;
    int id;
    float radians;
};