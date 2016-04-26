// main.cpp : Defines the entry point for the console application.
//

// may need #include "stdafx.h" in visual studio

// used for multi-threading
#include <process.h>
#include "ServerGame.h"
#include <iostream>

ServerGame * server;

void serverLoop(void * arg)
{
    while (true)
    {
        server->update();
    }
}

int main()
{
    server = new ServerGame();

    serverLoop((void*)12);
}

