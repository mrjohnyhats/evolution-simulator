#include <map>
#include <iostream>
#include <tuple>
#include <cmath.h>

#include "globals.h"
using namepspace std;

float Terrain::elvt_at_p(int x, int y){
	int px = fmod(x/10.0, 1.0);
	int py = fmod(y/10.0, 1.0);
	//get dist between px and py
	tuple<int, int> p = tmap[x/10][(int)fy];
	float diff = get<1>(p) - get<0>(p);
	return get<0>(p) + diff
}