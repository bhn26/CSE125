
#include "../../network/GameData.h"
#include "World.h"

using namespace std;

extern const int WORLD_WIDTH;
extern const int WORLD_HEIGHT;

typedef vector<PosInfo> pos_list;

class Engine {
public:
	Engine();
	~Engine();

	// generate map
	void InitWorld(int num_players);
	// move player
	// attack player

private:
	World * world; // used to communicate with clients

	/* maybe put the world representation in this class so we can 
	manipulate it and use ServerGame to send packets to clients*/

	pos_list GenerateCoords(int n); // generate random coordinates for n objs
};