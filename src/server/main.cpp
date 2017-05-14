// main.cpp : Defines the entry point for the console application.
//

// may need #include "stdafx.h" in visual studio

// used for multi-threading
#include <process.h>
#include "ServerGame.h"
#include "ConfigManager.h"
#include <iostream>

void ServerLoop(void* arg)
{
    while (true)
    {
        ServerGame::Instance()->Update();
    }
}

int main()
{
    ConfigManager::Instance()->LoadConfigs("eggs.cfg");

    ServerLoop(nullptr);
}

