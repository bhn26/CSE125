#include "../network/server/ServerGame.h"
#include "../network/server/GameData.h"

using namespace std;

typedef vector<PosInfo> pos_list;

class Engine {
public:
	Engine(ServerGame * g);
	~Engine();

	// generate map
	void InitMap();
	// move player
	// attack player

private:
	ServerGame * game; // used to communicate with clients

	/* maybe put the world representation in this class so we can 
	manipulate it and use ServerGame to send packets to clients*/

	pos_list GenerateCoords(int n); // generate random coordinates for n objs
}