#pragma once

enum MoveType {

    MOVE_UP = 0,

    MOVE_DOWN = 1,

    MOVE_LEFT = 2,

    MOVE_RIGHT = 3
};

class Dummy
{
public:
    Dummy(int newId, int newx, int newy);

    ~Dummy(void);
    
    void move(int direction);

    int getX();
    int getY();
    int getId();

private:
    int x;
    int y;
    int id;
};