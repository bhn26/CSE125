// main.cpp : Defines the entry point for the console application.
//

// may need #include "stdafx.h" in visual studio

// used for multi-threading
#include <process.h>
#include "ServerGame.h"
#include "ConfigManager.h"
#include <iostream>

void serverLoop(void * arg)
{
    while (true)
    {
        ServerGame::instance()->update();
    }
}

int main()
{
	ConfigManager::instantiate();
    ServerGame::instantiate();

    serverLoop((void*)12);
}

