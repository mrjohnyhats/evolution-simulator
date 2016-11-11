#ifndef TERRAIN_DEFINED
#define TERRAIN_DEFINED
#include <map>
#include <tuple>
#include <SDL2/SDL.h>

#include "globals.h"
using namespace std;

class Terrain {
	tuple<double, double> tmap[SCREEN_WIDTH/10][SCREEN_HEIGHT/10] = {{make_tuple(0,0)}};
	int m_quan, min_m_dist, m_radius;
	float m_height;
public:
	double next_elvt_at_p(double x, double y);
	void gen_rand();
	void log();
	void draw(SDL_Renderer* ren);
	void make_mountain(int radius, float height, int x, int y);
	Terrain();
};
#endif