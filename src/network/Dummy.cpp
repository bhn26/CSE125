#include "Dummy.h"
#include <stdio.h>

Dummy::Dummy(int newId, int newx, int newy)
{
    x = newx;
    y = newy;
    id = newId;

	radians = 0;
}

Dummy::~Dummy(void)
{
    
}

void Dummy::move(int direction)
{
    switch (direction)
    {
        case MOVE_FORWARD:
            y++;
            break;

        case MOVE_BACKWARD:
            y--;
            break;

        case MOVE_LEFT:
            x--;
            break;

        case MOVE_RIGHT:
            x++;
            break;

        default:
            break;
    }
}

void Dummy::move(int newx, int newy)
{
    x = newx;
    y = newy;
}

void Dummy::Rotate(float rad) {
    radians += rad;
	printf("rotated dummy on server");
}

int Dummy::getX()
{
    return x;
}

int Dummy::getY()
{
    return y;
}

int Dummy::getId()
{
    return id;
}