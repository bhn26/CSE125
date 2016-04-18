#include "Dummy.h"

Dummy::Dummy(int newId, int newx, int newy)
{
    x = newx;
    y = newy;
    id = newId;
}

Dummy::~Dummy(void)
{
    
}

void Dummy::move(int direction)
{
    switch (direction)
    {
        case MOVE_UP:
            y++;
            break;

        case MOVE_DOWN:
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