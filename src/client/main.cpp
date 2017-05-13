#ifdef _WIN32

// main.cpp : Defines the entry point for the console application.
//

// may need #include "stdafx.h" in visual studio

// used for multi-threading
//#define _WIN32
#include <process.h>
#include "ClientGame.h"
#include "ConfigManager.h"

int main()
{
    ClientGame::Instance()->GameLoop();
}
#endif      // _WIN32
