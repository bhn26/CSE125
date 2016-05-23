#ifdef _WIN32

// main.cpp : Defines the entry point for the console application.
//

// may need #include "stdafx.h" in visual studio

// used for multi-threading
//#define _WIN32
#include <process.h>
#include "ClientGame.h"
#include "ConfigManager.h"

//ClientGame * client;

int main()
{
	ConfigManager::instantiate();
    ClientGame::instantiate();
    ClientGame::instance()->GameLoop();
}
#endif      // _WIN32
