#ifndef TERRAIN_DEFINED
#define TERRAIN_DEFINED
#include <map>
#include <tuple>

#include "globals.h"
using namespace std;

class Terrain {
	tuple<int, int> tmap[SCREEN_WIDTH/10][SCREEN_HEIGHT/10];
public:
	float elvt_at_p(int x, int y);
	void ren_rand_pts();
	Terrain(map<string, float> opts);
}
#endif