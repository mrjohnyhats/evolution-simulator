#ifndef TERRAIN_DEFINED
#define TERRAIN_DEFINED
#include <map>
#include <tuple>

#include "globals.h"
using namespace std;

class Terrain {
	tuple<float, float> tmap[SCREEN_WIDTH/10][SCREEN_HEIGHT/10];
	int m_quan, min_m_dist, m_radius;
	float m_height;
public:
	float elvt_at_p(int x, int y);
	void gen_rand();
	void log();
	void make_mountain(int radius, float height, int x, int y);
	Terrain();
};
#endif