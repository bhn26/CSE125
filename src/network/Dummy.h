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

<<<<<<< 174b3f97e4b370bde6db6b896fe55f3188611e9a
    void Rotate(float rad);
=======
    void rotate(float v_rot, float h_rot);
>>>>>>> all rotation done by network and ignore *.db files

    int getX();
    int getY();
    float getVRotation() {return v_rotation; };
	float getHRotation() { return h_rotation; };
    int getId();

private:
    int x;
    int y;
    int id;
    float v_rotation;
	float h_rotation;
};