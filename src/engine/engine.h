#include "../network/server/ServerGame.h"

class Engine {
public:
	Engine(ServerGame * g);
	~Engine();

	// spawn players
	// spawn eggs
	// move player
	// attack player

private:
	ServerGame * game; // used to communicate with clients

	/* maybe put the world representation in this class so we can 
	manipulate it and use ServerGame to send packets to clients*/
}