// main.cpp : Defines the entry point for the console application.
//

// may need #include "stdafx.h" in visual studio

// used for multi-threading
#include <process.h>
#include "ServerGame.h"
#include "ConfigManager.h"
#include <iostream>

void serverLoop(void* arg)
{
    while (true)
    {
        ServerGame::Instance()->update();
    }
}

int main()
{
    ConfigManager::Instance()->LoadConfigs("eggs.cfg");

    serverLoop(nullptr);
}

