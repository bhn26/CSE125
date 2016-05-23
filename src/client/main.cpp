// main.cpp : Defines the entry point for the console application.
//

// may need #include "stdafx.h" in visual studio

// used for multi-threading
//#define _WIN32
#ifdef _WIN32
#include <process.h>
#endif
#include "ClientGame.h"

//ClientGame * client;

int main()
{
    ClientGame::instantiate();
    ClientGame::instance()->GameLoop();
}