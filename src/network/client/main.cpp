// main.cpp : Defines the entry point for the console application.
//

// may need #include "stdafx.h" in visual studio

// used for multi-threading
#include <process.h>
#include "ClientGame.h"

ClientGame * client;

void clientLoop()
{
    while (true)
    {
        //do game stuff
        client->update();
    }
}

int main()
{
    client = new ClientGame();

    clientLoop();
}