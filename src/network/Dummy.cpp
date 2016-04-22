#include "Dummy.h"
#include <stdio.h>

Dummy::Dummy(int newId, int newx, int newy)
{
    x = newx;
    y = newy;
    id = newId;

	v_rotation = 0;
	h_rotation = 0;
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

<<<<<<< 174b3f97e4b370bde6db6b896fe55f3188611e9a
void Dummy::Rotate(float rad) {
    radians += rad;
=======
void Dummy::rotate(float v_rot, float h_rot) {
    v_rotation += v_rot;
	h_rotation += h_rot;
>>>>>>> all rotation done by network and ignore *.db files
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